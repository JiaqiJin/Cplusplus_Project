#ifndef KAWAII_BODY_H
#define KAWAII_BODY_H

/*
	 A rigid body is the basic simulation object in the physics.
	 Torque = twisting force(moment). τ = Pf * f. Pf is point at force being applie. f is force.
	 τ = ad^ (axis representation).

	 Moment of inertia, change rotation speed. This depend mass and distance of the mass from axis rotation.
	 Torque—the rotational equivalent to force.
	 Ia = Σ(n, i = 1) Mi d^2[Pi->a]. dpi→a = distance of particle i form axis rotation a.
	 τ = I ¨o and ¨o = I^-1 * τ
*/

#include "core.h"

namespace Kawaii
{
	class RigidBody
	{
	public:

	protected:
		/*
		 Containing the same information we had in the Particle class,
		*/
		real inverseMass;
		//The amount of velocity the body loses each second.
		/*Holds the amount of damping applied to angular motion*/
		real angularDamping; //·O = ·O(Da)^t + ··Ot. da is the angular damping coefficient.
		/*Holds the amount of damping applied to lineal motion*/
		real linearDamping;
		Vector3 position;
		/*Hold the angular orientation of the rigid body in wolrd space.*/
		Quaternion orientation;
		Vector3 velocity;
		Vector3 rotation;
		Vector3 acceleration;
		/*Store current force, torque and acceleration of the rigid body.*/
		Vector3 forceAccum;
		/*Store accumulated torque to be applie at next integration step */
		Vector3 torqueAccum;
		/* Holds the linear acceleration of the rigid body, for the previus frame.*/
		Vector3 lastFrameAcceleration;
		/*Hold a transform matrix for converting body space into wolrd space and vice versa. */
		Matrix4 transformMatrix;
		/*Hold the inverse inertial tensor.The inertia tensor provided must not be degenerate */
		Matrix3 inverseInertiaTensor;
		/*Hold the inverse inertial tensor of the body in wolrd space. */
		Matrix3 inverseInertiaTensorWorld;

		/* A body can be put to sleep to avoid it being updated by the integration 
		functions or affected by collisions with the world.*/
		bool isAwake;
		/*Some bodies may never be allowed to fall asleep.*/
		bool canSleep;
		/*Holds the amount of motion of the body.*/
		real motion;

	public:
		/*
		 Calculates internal data from state data. 
		 This should be called after the body state is already directly.
		*/
		void calculateDerivedData();

		/*
		Integrates the rigid body forward in time by giving amount.
		 This function uses a Newton-Euler integration method, which is a linear approximation to the correct integral.
		*/
		void integrate(real duration);

		/*Add giving force to giving point on the rigid body.
		This is usefull for spring force, or other force fixed to the body.
		We need recalculate the position of the attachment each frame.
		*/
		void addForceAtBodyPoint(const Vector3& force, const Vector3& point);
		/*Adds the given force to the given point on the rigid body.*/
		void addForceAtPoint(const Vector3& force, const Vector3& point);
		/*Adds giving force to the center of the mass of the rigid body.*/
		void addForce(const Vector3& force);
		/*Applies the given change in velocity.*/
		void addVelocity(const Vector3& deltaVelocity);
		/*Applies the given change in rotation.*/
		void addRotation(const Vector3& deltaRotation);
		/*Adds the given torque to the rigid body.*/
		void addTorque(const Vector3& torque);
		/*Clear the force and torque in the accumulators.*/
		void clearAccumulators();

		/*
		Getter and Setter
		*/
		void setMass(const real mass);
		real getMass() const;
		void setInverseMass(const real inverseMass);
		real getInverseMass() const;
		/*return true if the mass of the body is not-inifinite.*/
		bool hasFiniteMass() const;
		void setInertiaTensor(const Matrix3& inertiaTensor);
		void getInertiaTensor(Matrix3* inertiaTensor) const;
		Matrix3 getInertiaTensor() const;
		void getInertiaTensorWorld(Matrix3* inertiaTensor) const;
		Matrix3 getInertiaTensorWorld() const;
		void setInverseInertiaTensor(const Matrix3& inverseInertiaTensor);
		void getInverseInertiaTensor(Matrix3* inverseInertiaTensor) const;
		Matrix3 getInverseInertiaTensor() const;
		void getInverseInertiaTensorWorld(Matrix3* inverseInertiaTensor) const;
		Matrix3 getInverseInertiaTensorWorld() const;
		void setPosition(const Vector3& position);
		void setPosition(const real x, const real y, const real z);
		void getPosition(Vector3* position) const;
		Vector3 getPosition() const;
		void setOrientation(const Quaternion& orientation);
		void setOrientation(const real r, const real i, const real j, const real k);
		void getOrientation(Quaternion* orientation) const;
		Quaternion getOrientation() const;
		void getOrientation(Matrix3* matrix) const;
		void getOrientation(real matrix[9]) const;
		void getTransform(Matrix4* transform) const;
		void getTransform(real matrix[16]) const;
		void getGLTransform(float matrix[16]) const;
		Matrix4 getTransform() const;
		/*Convert the giving point from the world space to body local space.*/
		Vector3 getPointInLocalSpace(const Vector3& point) const;
		/*Converts the given point from world space into the body local space. */
		Vector3 getPointInWorldSpace(const Vector3& point) const;
		Vector3 getDirectionInLocalSpace(const Vector3& direction) const;
		Vector3 getDirectionInWorldSpace(const Vector3& direction) const;
		void setVelocity(const Vector3& velocity);
		void setVelocity(const real x, const real y, const real z);
		void getVelocity(Vector3* velocity) const;
		Vector3 getVelocity() const;
		void setRotation(const Vector3& rotation);
		void setRotation(const real x, const real y, const real z);
		void getRotation(Vector3* rotation) const;
		Vector3 getRotation() const;
		bool getAwake() const{ return isAwake; }
		void setAwake(const bool awake = true);
		bool getCanSleep() const{ return canSleep; }
		void setCanSleep(const bool canSleep = true);
		void getLastFrameAcceleration(Vector3* linearAcceleration) const;
		Vector3 getLastFrameAcceleration() const;
		void setAcceleration(const Vector3& acceleration);
		void setAcceleration(const real x, const real y, const real z);
		void getAcceleration(Vector3* acceleration) const;
		Vector3 getAcceleration() const;
		void setDamping(const real linearDamping, const real angularDamping);
		void setLinearDamping(const real linearDamping);
		real getLinearDamping() const;
		void setAngularDamping(const real angularDamping);
		real getAngularDamping() const;
	};
}

#endif
