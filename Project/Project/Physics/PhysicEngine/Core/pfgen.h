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
	A force generator that applies a Spring force
	f =-k(|d|- l0) d
	*/
	class ParticleSpring : public ParticleForceGenerator
	{
	public:
		/* Creates a new spring with the given parameters. */
		ParticleSpring(Particle* other, real springConstant, real restLength);
		virtual void updateForce(Particle* particle, real duration);
	private:
		/* The particle at the other end of the spring. */
		Particle* other;
		/* Holds the sprint constant. */
		real springConstant;
		/* Holds the rest length of the spring. */
		real restLength;
	};

	/*
	 A force generator that applies a Spring force, where
	 one end is attached to a fixed point in space(no se mueve un extremo).
	*/
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	public:
		ParticleAnchoredSpring();
		ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength);
		const Vector3* getAnchor() const { return anchor; }
		/*Set the spring property*/
		void init(Vector3* anchor, real springConstant, real restLength);

		virtual void updateForce(Particle* particle, real duration);

	protected:
		/*The location of the anchored end of the spring*/
		Vector3* anchor;
		/*Hold the spring constant*/
		real springConstant;
		/* Holds the rest length of the spring. */
		real restLength;
	};

	/*
	 A force generator that applies a bungee force, where
	 one end is attached to a fixed point in space.
	*/
	class ParticleAnchoredBungee : public ParticleAnchoredSpring
	{
	public:
		//ParticleAnchoredBungee();
		virtual void updateForce(Particle* particle, real duration);
	private:
		Particle* other;
	};

	/*
	  A force generator that applies a buoyancy(flotabilidad) force for a plane of
	  liquid parrallel to XZ plane.
	  d = (yo − yw −s) /2s 
	  d = amount of the object sumerged.
	  yo = y coordinate of object.
	  yw = coordinate of the liquid plane.
	  s = submersion depth.
	*/
	class ParticleBuoyancy : public ParticleForceGenerator
	{
	public:
		ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f);
		virtual void updateForce(Particle* particle, real duration);
	private:
		/*The maximum submersion depth of a object*/
		real maxDepth;
		/*The volume of the object;*/
		real volume;
		/*The height of the water plane above*/
		real waterHeight;;
		/*The density of the liquid, pure water have densitt  1000kg*/
		real liquidDensity;
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
