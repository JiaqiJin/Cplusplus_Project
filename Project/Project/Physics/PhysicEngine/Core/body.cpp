#include "body.h"
#include <memory.h>
#include <assert.h>

using namespace Kawaii;


// --------------------INTERNAL OR HELPER FUNCTIONS: ----------------------

/*
 Inline function taht help create a transform matrix from a position and orientation.
*/
static inline void _calculateTransformMatrix(Matrix4& transformMatrix,
    const Vector3& position,
    const Quaternion& orientation)
{
    transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
        2 * orientation.k * orientation.k;
    transformMatrix.data[1] = 2 * orientation.i * orientation.j -
        2 * orientation.r * orientation.k;
    transformMatrix.data[2] = 2 * orientation.i * orientation.k +
        2 * orientation.r * orientation.j;
    transformMatrix.data[3] = position.x;

    transformMatrix.data[4] = 2 * orientation.i * orientation.j +
        2 * orientation.r * orientation.k;
    transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
        2 * orientation.k * orientation.k;
    transformMatrix.data[6] = 2 * orientation.j * orientation.k -
        2 * orientation.r * orientation.i;
    transformMatrix.data[7] = position.y;

    transformMatrix.data[8] = 2 * orientation.i * orientation.k -
        2 * orientation.r * orientation.j;
    transformMatrix.data[9] = 2 * orientation.j * orientation.k +
        2 * orientation.r * orientation.i;
    transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
        2 * orientation.j * orientation.j;
    transformMatrix.data[11] = position.z;
}

/*
Apply changes pf basic transformation to convert the constat inertial tensor
in object coordenates int othe corresponding matrix in thr WOLRD COORDINADES.

The transform matrix update to recalculate the derived quantity at each frame.
*/
static inline void _transformInertiaTensor(Matrix3& iitWorld,
    const Quaternion& q,
    const Matrix3& iitBody,
    const Matrix4& rotmat)
{
    real t4 = rotmat.data[0] * iitBody.data[0] +
        rotmat.data[1] * iitBody.data[3] +
        rotmat.data[2] * iitBody.data[6];
    real t9 = rotmat.data[0] * iitBody.data[1] +
        rotmat.data[1] * iitBody.data[4] +
        rotmat.data[2] * iitBody.data[7];
    real t14 = rotmat.data[0] * iitBody.data[2] +
        rotmat.data[1] * iitBody.data[5] +
        rotmat.data[2] * iitBody.data[8];
    real t28 = rotmat.data[4] * iitBody.data[0] +
        rotmat.data[5] * iitBody.data[3] +
        rotmat.data[6] * iitBody.data[6];
    real t33 = rotmat.data[4] * iitBody.data[1] +
        rotmat.data[5] * iitBody.data[4] +
        rotmat.data[6] * iitBody.data[7];
    real t38 = rotmat.data[4] * iitBody.data[2] +
        rotmat.data[5] * iitBody.data[5] +
        rotmat.data[6] * iitBody.data[8];
    real t52 = rotmat.data[8] * iitBody.data[0] +
        rotmat.data[9] * iitBody.data[3] +
        rotmat.data[10] * iitBody.data[6];
    real t57 = rotmat.data[8] * iitBody.data[1] +
        rotmat.data[9] * iitBody.data[4] +
        rotmat.data[10] * iitBody.data[7];
    real t62 = rotmat.data[8] * iitBody.data[2] +
        rotmat.data[9] * iitBody.data[5] +
        rotmat.data[10] * iitBody.data[8];

    iitWorld.data[0] = t4 * rotmat.data[0] +
        t9 * rotmat.data[1] +
        t14 * rotmat.data[2];
    iitWorld.data[1] = t4 * rotmat.data[4] +
        t9 * rotmat.data[5] +
        t14 * rotmat.data[6];
    iitWorld.data[2] = t4 * rotmat.data[8] +
        t9 * rotmat.data[9] +
        t14 * rotmat.data[10];
    iitWorld.data[3] = t28 * rotmat.data[0] +
        t33 * rotmat.data[1] +
        t38 * rotmat.data[2];
    iitWorld.data[4] = t28 * rotmat.data[4] +
        t33 * rotmat.data[5] +
        t38 * rotmat.data[6];
    iitWorld.data[5] = t28 * rotmat.data[8] +
        t33 * rotmat.data[9] +
        t38 * rotmat.data[10];
    iitWorld.data[6] = t52 * rotmat.data[0] +
        t57 * rotmat.data[1] +
        t62 * rotmat.data[2];
    iitWorld.data[7] = t52 * rotmat.data[4] +
        t57 * rotmat.data[5] +
        t62 * rotmat.data[6];
    iitWorld.data[8] = t52 * rotmat.data[8] +
        t57 * rotmat.data[9] +
        t62 * rotmat.data[10];
}

/*
Each frame, we calculate the transform matrix, transform the inverse inertial tensor into
world coordinates, and performance the rigid body integration.
*/
void RigidBody::calculateDerivedData()
{
	orientation.normalise();

    //calculate the transform matrix for the rigid body.
    _calculateTransformMatrix(transformMatrix, position, orientation);

    // Calculate the inertia tensor in wolrd space.
    _transformInertiaTensor(inverseInertiaTensorWorld, orientation,
        inverseInertiaTensor, transformMatrix);
}

void RigidBody::setInertiaTensor(const Matrix3& inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
}
