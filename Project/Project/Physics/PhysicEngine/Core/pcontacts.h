#ifndef KAWAII_PCONTACTS_H
#define KAWAII_PCONTACTS_H

#include "particle.h"

namespace Kawaii
{
	/*
	A contact represent 2 obj in contact. 
	Resolving acontact removes their interpenetration, and applies impulse to keep them apart.
	Colliding bodies may also rebound.
	vs = (`pa - `pb) · n
	vs` = -c * vs (c = restitution coefficent)
	*/
	class ParticleContact
	{
	public:
		/*Hold the particle that are involved in the contact*/
		Particle* particle[2];
		/*Hold the normal restitution coefficent*/
		real restitution;
		/*Hold the direction of the contact in the world*/
		Vector3 contactNormal;

	protected:
		/* Handle the impulse calculation for this impulsion*/
		void resolve(real duration);
		/* Calculate the separate velocity at this contact*/
		real calculateSeparatingVelocity() const;

	private:
		/* Handles the impulse calculations for this collision.*/
		void resolveVelocity(real duration);
	};
}

#endif