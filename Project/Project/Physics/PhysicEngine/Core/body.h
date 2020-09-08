#ifndef KAWAII_BODY_H
#define KAWAII_BODY_H

#include "core.h"

namespace Kawaii
{
	/*
	 A rigid body is the basic simulation object in the physics.
	*/
	class RigidBody
	{
	public:

	protected:
		/*
		 Containing the same information we had in the Particle class,
		*/
		real inverseMass;
		Vector3 position;
		/*Hold the angular orientation of the rigid body in wolrd space.*/
		Quaternion orientation;
		Vector3 velocity;
		Vector3 rotation;
		/*Hold a transform matrix for converting body space into wolrd space and vice versa. */
		Matrix4 transformMatrix;
		/*Hold the inverse inertial tensor.The inertia tensor provided must not be degenerate */
		Matrix3 inverseInertiaTensor;
		/*Hold the inverse inertial tensor of the body in wolrd space. */
		Matrix3 inverseInertiaTensorWorld;
	public:
		/*
		 Calculates internal data from state data. 
		 This should be called after the body state is already directly.
		*/
		void calculateDerivedData();

		/*Sets the intertia tensor for the rigid body.*/
		void setInertiaTensor(const Matrix3& inertiaTensor);
	};
}

#endif
