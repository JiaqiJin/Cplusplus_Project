#include "collide_fine.h"

using namespace Kawaii;

bool IntersectionTests::sphereAndSphere(
    const CollisionSphere& one,
    const CollisionSphere& two)
{
    // Find the vector between the objects
    Vector3 midline = one.getAxis(3) - two.getAxis(3);

    // See if it is large enough.
    return midline.squareMagnitude() < (one.radius + two.radius) * (one.radius + two.radius);
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
