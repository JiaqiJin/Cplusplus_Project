#include "BoundingSphere.h"

using namespace Kawaii;

BoundingSphere::BoundingSphere(const Vector3& centre, real radius)
{
    BoundingSphere::centre = centre;
    BoundingSphere::radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& one,
    const BoundingSphere& two)
{
    //TODO
}

bool BoundingSphere::overlaps(const BoundingSphere* other) const
{
    real distanceSquared = (centre - other->centre).squareMagnitude();
    return distanceSquared < (radius + other->radius) * (radius + other->radius);
}

real BoundingSphere::getGrowth(const BoundingSphere& other) const
{
    BoundingSphere newSphere(*this, other);
    //Return the value proportional surface area of sphere.
    return newSphere.radius * newSphere.radius - radius * radius;
}