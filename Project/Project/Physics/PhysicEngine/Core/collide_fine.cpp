#include "collide_fine.h"

using namespace Kawaii;

/*
Separating Axes 
two objects cannot possibly be in contact as long as there is some axis on which the obj can be projected.
1- We choose the axis.
2- we project the objs onto this axis.
3- we check to see whether the projection are overlapping.
*/
static inline real transformToAxis(
    const CollisionBox& box,
    const Vector3& axis
)
{
    return
        box.halfSize.x * real_abs(axis * box.getAxis(0)) +
        box.halfSize.y * real_abs(axis * box.getAxis(1)) +
        box.halfSize.z * real_abs(axis * box.getAxis(2));
}

/*
This function checks if the two boxes overlap along the giving axis.
The final parameter toCentre is used to pass in the vector betwwen the box center points.
*/
static inline bool overlapOnAxis(
    const CollisionBox& one,
    const CollisionBox& two, 
    const Vector3& axis,
    const Vector3& toCentre)
{
    //Projection the half size of one onto axis
    real oneProject = transformToAxis(one, axis);
    real twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    real distance = real_abs(toCentre * axis);

    //Check overlap
    return (distance < oneProject + twoProject);
}

//--------------------------------

bool IntersectionTests::sphereAndSphere(
    const CollisionSphere& one,
    const CollisionSphere& two)
{
    // Find the vector between the objects
    Vector3 midline = one.getAxis(3) - two.getAxis(3);

    // See if it is large enough.
    return midline.squareMagnitude() < (one.radius + two.radius) * (one.radius + two.radius);
}

bool IntersectionTests::sphereAndHalfSpace(
    const CollisionSphere& sphere,
    const CollisionPlane& plane)
{
    // Find the distance from the origin
    real ballDistance =
        plane.direction *
        sphere.getAxis(3) -
        sphere.radius;

    // Check for the intersection
    return ballDistance <= plane.offset;
}

bool IntersectionTests::boxAndHalfSpace(
    const CollisionBox& box,
    const CollisionPlane& plane)
{
    // Work out the projected radius of the box onto the plane direction
    real projectedRadius = transformToAxis(box, plane.direction);

    // Work out how far the box is from the origin
    real boxDistance =
        plane.direction *
        box.getAxis(3) -
        projectedRadius;

    // Check for the intersection
    return boxDistance <= plane.offset;
}

unsigned CollisionDetector::sphereAndHalfSpace(
    const CollisionSphere& sphere,
    const CollisionPlane& plane,
    CollisionData* data)
{
    if (data->contactsLeft <= 0)return 0;

    //Cache the sphere position
    Vector3 position = sphere.getAxis(3);

    //Find the distance form the plane
    real ballDistance = plane.direction * position - sphere.radius - plane.offset;

    if (ballDistance >= 0) return 0;

    // Create the contact - it has a normal in the plane direction.
    Contact* contact = data->contacts;
    contact->contactNormal = plane.direction;
    contact->penetration = -ballDistance;
    contact->contactPoint = position - plane.direction * (ballDistance + sphere.radius);
    contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::sphereAndTruePlane(
    const CollisionSphere& sphere,
    const CollisionPlane& plane,
    CollisionData* data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    // Cache the sphere position
    Vector3 position = sphere.getAxis(3);

    // Find the distance from the plane
    real centreDistance = plane.direction * position - plane.offset;

    // Check if we're within radius
    if (centreDistance * centreDistance > sphere.radius * sphere.radius) { return 0; }

    // Check which side of the plane we're on
    Vector3 normal = plane.direction;
    real penetration = -centreDistance;
    if (centreDistance < 0)
    {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.radius;

    //Create the contact
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = position - plane.direction * centreDistance;
    contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::sphereAndSphere(
    const CollisionSphere& one,
    const CollisionSphere& two,
    CollisionData* data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    //Cache the shepre position
    Vector3 positionOne = one.getAxis(3);
    Vector3 positionTwo = two.getAxis(3);

    // Find the vector between the objects
    Vector3 midline = positionOne - positionTwo;
    real size = midline.magnitude();

    // See if it is large enough.
    if (size <= 0.0f || size >= one.radius + two.radius)
    {
        return 0;
    }

    Vector3 normal = midline * (((real)1.0) / size);
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = positionOne + midline * (real)0.5;
    contact->penetration = (one.radius + two.radius - size);
    
    contact->setBodyData(one.body, two.body, data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(
    const CollisionBox& box,
    const CollisionPlane& plane,
    CollisionData* data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    //Check for intersection
    if (IntersectionTests::boxAndHalfSpace(box, plane))
    {
        return 0;
    }
    /*
     We have an intersection, so find the intersection points. We can make
     do only checking vertices. If the vox is resting on a plane
     or on edge, it will be reported as 4 or 2 contact point.
    */

    // Go through each combination of + and - for each half-size
    static real mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                              {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

    Contact* contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++)
    {
        // Calculate the position of each vertex
        Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos.componentProductUpdate(box.halfSize);
        vertexPos = box.transform.transform(vertexPos);

        // Calculate the distance from the plane
        real vertexDistance = vertexPos * plane.direction;

        //Compare to plane distance
        if (vertexDistance <= plane.offset)
        {
            //The contact point is halfway between the vertex and the
            // plane - we multiply the direction by half the separation
            // distance and add the vertex location.
            contact->contactPoint = plane.direction;
            contact->contactPoint *= (vertexDistance - plane.offset);
            contact->contactPoint += vertexPos;
            contact->contactNormal = plane.direction;
            contact->penetration = plane.offset - vertexDistance;

            // Write the appropriate data
            contact->setBodyData(box.body, NULL,
                data->friction, data->restitution);

            // Move onto the next contact
            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned)data->contactsLeft) return contactsUsed;
        }
    }
    data->addContacts(contactsUsed);
    return contactsUsed;
}

/*
face–face contact,
an edge–face contact, or a point–face contact. In each case the sphere (which has no
edges or vertices) contributes a face to the contact.
Fortunately all three of these cases involve the same calculations for the normal
contact and penetration depth.

Separating Axes
If we find any direction in space in which 2 objs are not colliding, then the 2 objs are nopt colliding at all.
we can simply test the three axes of the box and check if the point is too far away to be colliding.

Contact Generation
FInd the closest point in the box to the target point and generate the contact form it.
Clamp each component of the test point to the half-size of the box in the same direction.
With the new point we can then work out the distance form the center of the spehre to the target point.
*/
unsigned CollisionDetector::boxAndSphere(
    const CollisionBox& box,
    const CollisionSphere& sphere,
    CollisionData* data)
{
    // Transform the centre of the sphere into box coordinates
    //we’ve taken into account both the orientation of the box and its center.
    Vector3 centre = sphere.getAxis(3);
    Vector3 relCentre = box.transform.transformInverse(centre);

    // Early-out check to see if we can exclude the contact.Separating Axes
    if (real_abs(relCentre.x) - sphere.radius > box.halfSize.x ||
        real_abs(relCentre.y) - sphere.radius > box.halfSize.y ||
        real_abs(relCentre.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    Vector3 closestPt(0, 0, 0);
    real dist;

    //Clapm each coordinate to the box
    dist = relCentre.x;
    if (dist > box.halfSize.x) dist = box.halfSize.x;
    if (dist < -box.halfSize.x) dist = -box.halfSize.x;
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.halfSize.y) dist = box.halfSize.y;
    if (dist < -box.halfSize.y) dist = -box.halfSize.y;
    closestPt.y = dist;

    dist = relCentre.z;
    if (dist > box.halfSize.z) dist = box.halfSize.z;
    if (dist < -box.halfSize.z) dist = -box.halfSize.z;
    closestPt.z = dist;

    //Check we are in conatct
    dist = (closestPt - relCentre).squareMagnitude();
    if (dist > sphere.radius * sphere.radius) return 0;

    // Compile the contact, The contact properties need to be given in world coordinates.
    //we need to find the closest point in world coordinates, transforming the point
    Vector3 closestPtWorld = box.transform.transform(closestPt);

    Contact* contact = data->contacts;
    contact->contactNormal = (closestPtWorld - centre);
    contact->contactNormal.normalise();
    contact->contactPoint = closestPtWorld;
    contact->penetration = sphere.radius - real_sqrt(dist);
    contact->setBodyData(box.body, sphere.body,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

/*
Consider each vertex of obj A.
Calculate the interpenetration of that vertex with obj B.
The deepest such interpenetration is retained.
Do the Same with obj B vertex against obj A.
The deepest interpenetration overall is reatined.
*/
unsigned CollisionDetector::boxAndPoint(
    const CollisionBox& box,
    const Vector3& point,
    CollisionData* data)
{
    //Transform the point into box coordinates
    Vector3 relPt = box.transform.transformInverse(point);
    Vector3 normal;
    // Check each axis, looking for the axis on which the
    // penetration is least deep.
    //x
    real min_depth = box.halfSize.x - real_abs(relPt.x);
    if (min_depth < 0) return 0;
    normal = box.getAxis(0) * ((relPt.x < 0) ? -1 : 1);
    //y
    real depth = box.halfSize.y - real_abs(relPt.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * ((relPt.y < 0) ? -1 : 1);
    }
    //z
    depth = box.halfSize.z - real_abs(relPt.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * ((relPt.z < 0) ? -1 : 1);
    }

    // Compile the contact
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

    contact->setBodyData(box.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}
