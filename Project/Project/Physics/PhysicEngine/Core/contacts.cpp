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