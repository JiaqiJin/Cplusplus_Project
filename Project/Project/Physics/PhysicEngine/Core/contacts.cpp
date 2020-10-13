#include "contacts.h"

using namespace Kawaii;

void Contact::setBodyData(RigidBody* one, RigidBody* two,
    real friction, real restitution)
{
    Contact::body[0] = one;
    Contact::body[1] = two;
    Contact::friction = friction;
    Contact::restitution = restitution;
}

/*
This is stored as a 3x3 matrix, where each vector is a column.
The x direction is generated from the contact normal, and the y and z directions 
are set so they are at right angles to it.
*/
void Contact::calculateContactBasis()
{
    Vector3 contactTangent[2];

    //Check whether the Z - axis is nearer to the X or Y axis
    if (real_abs(contactNormal.x) > real_abs(contactNormal.y))
    {
        //Scaling dactor ro ensure the result are normalised
        const real s = (real)1.0f / real_sqrt(contactNormal.z * contactNormal.z +
            contactNormal.x * contactNormal.x);

        // The new X-axis is at right angles to the world Y-axis
        contactTangent[0].x = contactNormal.z * s;
        contactTangent[0].y = 0;
        contactTangent[0].z = -contactNormal.x * s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = contactNormal.y * contactTangent[0].x;
        contactTangent[1].y = contactNormal.z * contactTangent[0].x - contactNormal.x * contactTangent[0].z;
        contactTangent[1].z = -contactNormal.y * contactTangent[0].x;
    }
    else
    {
        // Scaling factor to ensure the results are normalised
        const real s = (real)1.0 / real_sqrt(contactNormal.z * contactNormal.z +
            contactNormal.y * contactNormal.y);
        // The new X-axis is at right angles to the world X-axis
        contactTangent[0].x = 0;
        contactTangent[0].y = -contactNormal.z * s;
        contactTangent[0].z = contactNormal.y * s;

        // The new Y-axis is at right angles to the new X- and Z- axes
        contactTangent[1].x = contactNormal.y * contactTangent[0].z -
            contactNormal.z * contactTangent[0].y;
        contactTangent[1].y = -contactNormal.x * contactTangent[0].z;
        contactTangent[1].z = contactNormal.x * contactTangent[0].y;
    }

    // Make a matrix from the three vectors.
    contactToWorld.setComponents(contactNormal, contactTangent[0], contactTangent[1]);
}

/*
For contacts with 2 objs invilved we have 4 value : 
- vel caused by lineal and angunar motion for each objs
For the contact we only have Rigid bodie.

The Linear Component : holds only for the linear component of velocity
 Δ`Pd = m(a)^-1 + m(b)^-1

 The Angular Component
 Qrel = pos of the contact relative to the origin of a obj.
Qrel = q - p
u = Qrel x ^d <(TorquePerUnitImpulse = relativeContactPosition % normal )>
ΔÒ = I^-1 · u <[ rotationPerUnitImpulse = inverseInertiaTensor.transform(torquePerUnitImpulse)]>
the total velocity of a point
`q = Ò X Qrel <[velocityPerUnitImpulse = rotationPerUnitImpulse % TorquePerUnitImpulse]>

Velocity, velocityPerUnitImpulseContact(transformTranspose(velocityPerUnitImpulse);))

The rotation induced velocty of a point (`q) depend on its position relative to the origin of the obj (q - p)
and on the objs angular velocity (Ò).
The result will be the velocity caused by rotation per unit impulse.
The velocity is in world space, we interesting along the contact normal.

-VELOCITY CHANGE BY IMPULSE : Impulses cause a change in velocity, calculate the impulse at the collision
-IMPULSE CHANGE BY VELOCITY : CALCULATING THE DESIRED VELOCITY CHANGE, CALCULATING THE IMPULSE, APPLYING THE IMPULSE
*/
inline
Vector3 Contact::calculateFrictionlessImpulse(Matrix3* inverseInertiaTensor)
{
    Vector3 impulseContact;

    // Build a vector that shows the change in velocity in
    // world space for a unit impulse in the direction of the contact
    // normal.
    Vector3 deltaVelWorld = relativeContactPosition[0] % contactNormal;
    deltaVelWorld = inverseInertiaTensor[0].transform(deltaVelWorld);
    deltaVelWorld = deltaVelWorld % relativeContactPosition[0];

    // Work out the change in velocity in contact coordiantes.
    real deltaVelocity = deltaVelWorld * contactNormal;

    // Add the linear component of velocity change
    deltaVelocity += body[0]->getInverseMass();

    // Check if we need to the second body's data
    if (body[1])
    {
        // Go through the same transformation sequence again
        Vector3 deltaVelWorld = relativeContactPosition[1] % contactNormal;
        deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
        deltaVelWorld = deltaVelWorld % relativeContactPosition[1];

        // Add the change in velocity due to rotation
        deltaVelocity += deltaVelWorld * contactNormal;

        // Add the change in velocity due to linear motion
        deltaVelocity += body[1]->getInverseMass();
    }

    // Calculate the required size of the impulse
    impulseContact.x = desiredDeltaVelocity / deltaVelocity;
    impulseContact.y = 0;
    impulseContact.z = 0;
    return impulseContact;
}


/*
CALCULATING THE DESIRED VELOCITY CHANGE :
Calculating the Closing Velocity : velocity has both a linear and an angular component. To calculate
the total velocity of one object at the contact point we need both.
 We can retrieve the linear velocity from an object directly, it is stored in the rigid body.

V`s = -CVs => ΔVs = -Vs - CVs = (-1+C)Vs
real deltaVelocity = -contactVelocity.x * (1 + restitution);

We need remove all existing closing velocity at the contact, and keep going so the final velocity c times its 
original value but in the oposite direction.
If the coefficient of restitution, c, is zero, then the change in velocity will be sufficient
to remove all the existing closing velocity but no more.
*/
void Contact::calculateDesiredDeltaVelocity(real duration)
{
    const static real velocityLimit = (real)0.25f;

    //Calculate the acceleration induced velocity accumulate this frame
    real velocityFromAcc = 0;

    if (body[0]->getAwake())
    {
        velocityFromAcc += body[0]->getLastFrameAcceleration() * duration * contactNormal;
    }

    if (body[1] && body[1]->getAwake())
    {
        velocityFromAcc -=  body[1]->getLastFrameAcceleration() * duration * contactNormal;
    }
    // If the velocity is very slow, limit the restitution
    real thisRestitution = restitution;
    if (real_abs(contactVelocity.x) < velocityLimit)
    {
        thisRestitution = (real)0.0f;
    }

    // Combine the bounce velocity with the removed acceleration velocity.
    desiredDeltaVelocity = -contactVelocity.x - thisRestitution * (contactVelocity.x - velocityFromAcc);
}

/*
Calculating and  Applying the impulse
In contact coordenade the contact normal is in X axis G(contact) = [g ,0 , 0]. G is the impulse
convert out of contact coordinates into world coordinates.<[gworld = Mgcontact]>, Convert impulse to world coordinates.

`p = g / m velocity change
rotation change = ΔÒ = I^-1 · u 
u = qrel × g

*/
void Contact::applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
    //Get hold of the inverse mass and inverse inertia tensor, both in world coordinates
    Matrix3 inverseInertiaTensor[2];
    body[0]->getInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
    if (body[1])
    {
        body[1]->getInverseInertiaTensorWorld(&inverseInertiaTensor[1]);
    }

    // We will calculate the impulse for each contact axis
    Vector3 impulseContact;

    if (friction == (real)0.0)
    {
        impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
    }
    else
    {
        // Otherwise we may have impulses that aren't in the direction of the
        // contact, so we need the more complex version.
        //TODO
    }
    //Converte impulse to wolrd coordinates
    Vector3 impulse = contactToWorld.transform(impulseContact);

    // Split in the impulse into linear and rotational components
    //u = qrel × g (g = impulse)
    //ΔÒ = I^-1 · u 
    // `p = g / m
    Vector3 impulsiveTorque = relativeContactPosition[0] % impulse;
    rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
    velocityChange[0].clear();
    velocityChange[0].addScaledVector(impulse, body[0]->getInverseMass());

    // Apply the changes
    body[0]->addVelocity(velocityChange[0]);
    body[0]->addRotation(rotationChange[0]);

    if (body[1])
    {
        // Work out body one's linear and angular changes
        Vector3 impulsiveTorque = impulse % relativeContactPosition[1];
        rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
        velocityChange[1].clear();
        // opposite direction respective to 1 obj.
        velocityChange[1].addScaledVector(impulse, -body[1]->getInverseMass());

        // And apply them.
        body[1]->addVelocity(velocityChange[1]);
        body[1]->addRotation(rotationChange[1]);
    }
}

/*
Ìnterpenetration : IMPLEMENTING NONLINEAR PROJECTION 
Calculating the Components : 
Inertial : the resistance of an object TO BEING MOVING
inertia will have a linear component and a rotational component.
The linear component of inertia = INVERSE MASS
The angular component previus used.

Applying the Movement 
Angular motion : 1 calculate the rotation needed to move the contact point by one unit.
2 multiply this by number of unit needed
3 apply the rotation to the orientation quartenion.


*/
void Contact::applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration)
{
    const real angularLimit = (real)0.2f;
    real angularMove[2];
    real linearMove[2];

    real totalInertia = 0;
    real linearInertia[2];
    real angularInertia[2];
    // We need to work out the inertia of each object in the direction
    // of the contact normal, due to angular inertia only.
    for (unsigned i = 0; i < 2; i++)
    {
        if (body[i])
        {
            Matrix3 inverseInertiaTensor;
            body[i]->getInverseInertiaTensorWorld(&inverseInertiaTensor);

            //Use the same procedure as for calculating frictionless
             // velocity change to work out the angular inertia.
            Vector3 angularInertiaWorld = relativeContactPosition[i] % contactNormal;
            angularInertiaWorld = inverseInertiaTensor.transform(angularInertiaWorld);
            angularInertiaWorld = angularInertiaWorld % relativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld * contactNormal;

            // The linear component is simply the inverse mass
            linearInertia[i] = body[i]->getInverseMass();

            //keep track of total inertial from all component
            totalInertia += linearInertia[i] + angularInertia[i];
            // We break the loop here so that the totalInertia value is
            // completely calculated (by both iterations) before
            // continuing.
        }
    }

    // Loop through again calculating and applying the changes
    for (unsigned i = 0; i < 2; i++) if (body[i])
    {

    }
}