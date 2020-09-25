#ifndef KAWAII_FGEN_H
#define KAWAII_FGEN_H

#include "body.h"
#include "pfgen.h"
#include <vector>

namespace Kawaii
{
	class ForceGenerator
	{
	public:
		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

	class Gravity : public ForceGenerator
	{
	public:
		Gravity(const Vector3& gravity);
		virtual void updateForce(RigidBody* body, real duration);

	private:
		Vector3 gravity;
	};

	/*A force generator that applies a Spring force.
	We need know where the spring attach to each obj.
	*/
	class Spring : public ForceGenerator
	{
	public:
		Spring(const Vector3& localConnectionPt,
			RigidBody* other,
			const Vector3& otherConnectionPt,
			real springConstant,
			real restLength);
		virtual void updateForce(RigidBody* body, real duration);
	private:
		/*Connection of the spring.*/
		Vector3 connectionPoint;
		Vector3 otherConnectionPoint;
		RigidBody* other;
		real springConstant;
		real restLength;
	};

}

#endif