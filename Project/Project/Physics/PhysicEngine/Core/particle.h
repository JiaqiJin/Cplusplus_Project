#ifndef KAWAII_PARTICLE_H
#define KAWAII_PARTICLE_H

#include "core.h"

namespace Kawaii
{
	/*
	Simple object that can be simulate in the physic system.
	*/
	class Particle
	{
	public:

	protected:
		/*Hold the linear of damping apply to linear motion*/
		real damping;
		/**/
		real inverseMass;
		/*Linear position of the particle in world space*/
		Vector3 position;
		/*Lineal velocity of the particle in the world space*/
		Vector3 velocity;
		/*Acceleration of the particle, can be used tp set acceleration due to gravity*/
		Vector3 acceleration;
		/*Hold accumulated force*/
		Vector3 forceAccum;
	public:
		void integrate(real duration);
		/*
		Setter
		*/
		void setPosition(const Vector3& position);
		void setPosition(const real x, const real y, const real z);

		void setVelocity(const Vector3& velocity);
		void setVelocity(const real x, const real y, const real z);

		void setAcceleration(const Vector3& acceleration);
		void setAcceleration(const real x, const real y, const real z);

		void setMass(const real mass);
		void setInverseMass(const real inverseMass);

		void setDamping(const real damping);

		/*
		Getter
		*/
		//position
		void getPosition(Vector3* position) const;
		Vector3 getPosition() const;
		//velocity
		void getVelocity(Vector3* velocity) const;
		Vector3 getVelocity() const;
		//acceleration
		void getAcceleration(Vector3* acceleration) const;
		Vector3 getAcceleration() const;
		//mass
		real getMass() const;
		real getInverseMass() const;
		//damping
		real getDamping() const;

		void clearAccumulator();
		void addForce(const Vector3& force);
		bool hasFiniteMass() const;

	private:

	};
}

#endif
