#include "pcontacts.h"

using namespace Kawaii;

void ParticleContact::resolve(real duration)
{
	resolveVelocity(duration);
}

/*
vs = (`pa - `pb) · n
*/
real ParticleContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
	return relativeVelocity * contactNormal;
}

/*
vs = (`pa - `pb) · n

Vs` = -c (coeff restitution) * vs.

impulse f = m``p.

force g = m * `p (here we use inverse mass)

particle velocity `p = 1/m * g
*/
void ParticleContact::resolveVelocity(real duration)
{
	// Find the velocity in the direction of the contact
	real separatingVelocity = calculateSeparatingVelocity();

	//check if it need to be resolved
	if (separatingVelocity > 0)
	{
		// The contact is either separating, or stationary - there's
        // no impulse required.
		return;
	}

	// Calculate the new separating velocity (vs`)
	real newSepVelocity = -separatingVelocity * restitution;

	// Check the velocity build-up due to acceleration only (acceleration)
	Vector3 accCausedVelocity = particle[0]->getVelocity();
	if (particle[1]) accCausedVelocity -= particle[1]->getAcceleration();
	real accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

	// If we've got a closing velocity due to acceleration build-up,
    // remove it from the new separating velocity
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitution * accCausedSepVelocity;
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;

	// We apply the change in velocity to each object in proportion to
	// their inverse mass (i.e. those with lower inverse mass [higher
	// actual mass] get less change in velocity).
	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	// If all particles have infinite mass, then impulses have no effect
	if (totalInverseMass <= 0) return;

	// Calculate the impulse to apply
	real impulse = deltaVelocity / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	Vector3 impulsePerIMass = contactNormal * impulse;

	// Apply impulses: they are applied in the direction of the contact,
	// and are proportional to the inverse mass.
	particle[0]->setVelocity(particle[0]->getVelocity() +
							 impulsePerIMass * particle[0]->getInverseMass());

	if (particle[1])
	{
		// Particle 1 goes in the opposite direction
		particle[1]->setVelocity(particle[1]->getVelocity() +
			impulsePerIMass * -particle[1]->getInverseMass());
	}
}