#ifndef KAWAII_PCONTACTS_H
#define KAWAII_PCONTACTS_H

#include "particle.h"

namespace Kawaii
{
	class ParticleContactResolver;

	/*
	A contact represent 2 obj in contact. 
	Resolving acontact removes their interpenetration, and applies impulse to keep them apart.
	Colliding bodies may also rebound.
	vs = (`pa - `pb) · n
	vs` = -c * vs (c = restitution coefficent)
	*/
	class ParticleContact
	{
		friend ParticleContactResolver;
	public:
		/*Hold the particle that are involved in the contact*/
		Particle* particle[2];
		/*Hold the normal restitution coefficent*/
		real restitution;
		/*Hold the direction of the contact in the world*/
		Vector3 contactNormal;
		/* Holds the depth of penetration at the contact */
		real penetration;

	protected:
		/* Handle the impulse calculation for this impulsion*/
		void resolve(real duration);
		/* Calculate the separate velocity at this contact*/
		real calculateSeparatingVelocity() const;

	private:
		/* Handles the impulse calculations for this collision.*/
		void resolveVelocity(real duration);
	};

	/*
	The contact solution routine for particle contact.
	*/
	class ParticleContactResolver
	{
	protected:
		/*Hold the number of iteration*/
		unsigned iterations;
		/*recording the actural number of iterations use*/
		unsigned iterationsUsed;

	public:
		ParticleContactResolver(unsigned iterations);

		/*set number of iteration*/
		void setIterations(unsigned iterations);

		/*Resolves a set of particle contacts for both penetration and velocity.*/
		void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
	};

	class ParticleContactGenerator
	{
	public:
		/*
		* Fill the giving contact structure with the generated contact.
		*/
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};

}

#endif