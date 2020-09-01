#ifndef KAWAII_PFGEN_H
#define KAWAII_PFGEN_H

#include <vector>
#include "core.h"
#include "particle.h"

namespace Kawaii
{
	/*
	A force generator can add a force to one or more particles
	*/
	class ParticleForceGenerator
	{
	public:
		virtual void updateForce(Particle* particle, real duration) = 0;
	};

	/*
	A force generator to apply a gravity force
	*/
	class ParticleGravity : public ParticleForceGenerator
	{
	public:
		ParticleGravity(const Vector3& gravity);

		virtual void updateForce(Particle* particle, real duration);
	private:
		Vector3 gravity;
	};

	/*
	Force generator that apply to a drag force
	fdrag =-ÿp(k1|ÿp| + k2|ÿp|^2)
	*/
	class ParticleDrag : public ParticleForceGenerator
	{
	public:
		ParticleDrag(real k1, real k2);

		virtual void updateForce(Particle* particle, real duration);
	private:
		/** Holds the velocity drag coeffificent. */
		real k1;
		/** Holds the velocity squared drag coeffificent. */
		real k2;
	};


	/*
	Hold all the force generator and the particle it applies to.
	*/
	class ParticleForceRegistry
	{
	protected:

		struct ParticleForceRegistration
		{
			Particle* particle;
			ParticleForceGenerator* fg;
		};
		/*Hold list of the registrations*/
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;

	public:

		void add(Particle* particle, ParticleForceGenerator* fg);
		/*Remove given register pair.*/
		void remove(Particle* particle, ParticleForceGenerator* fg);
		/*Clear all registration form the registery*/
		void clear();
		/*Call all force generators to update the force*/
		void updateForces(real duration);
	};

}

#endif
