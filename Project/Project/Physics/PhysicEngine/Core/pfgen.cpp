#include "pfgen.h"
#include <assert.h>

using namespace Kawaii;

void ParticleForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i < registrations.end(); i++)
	{
		i->fg->updateForce(i->particle, duration);
	}
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	assert(particle != nullptr && fg != nullptr);

	Registry::iterator i = registrations.begin();
	for (; i < registrations.end(); i++)
	{
		if (i->fg == fg && i->particle == particle)
		{
			registrations.erase(i);
		}
	}
}

void ParticleForceRegistry::clear()
{
	registrations.clear();
}

/*--------------------Particle Gravity-----------*/

ParticleGravity::ParticleGravity(const Vector3& gravity)
	:gravity(gravity) {}

void ParticleGravity::updateForce(Particle* particle, real duration)
{
	// Check that we do not have infinite mass
	if (!particle->hasFiniteMass()) return;

	// Apply the mass-scaled force to the particle
	particle->addForce(gravity * particle->getMass());
}

/*--------------------DRAG FORCE GENERATOR ----------*/

ParticleDrag::ParticleDrag(real k1, real k2)
	:k1(k1),k2(k2)
{

}
/*fdrag =-ÿp(k1|ÿp| + k2|ÿp|^2)*/
void ParticleDrag::updateForce(Particle* particle, real duration)
{
	Vector3 force;
	//*velocity = Particle::velocity; assigned particle velocity to a force
	particle->getVelocity(&force);
	// Calculate the total drag coefficient
	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// Calculate the final force and apply it
	force.normalise();
	force *= -dragCoeff;
	particle->addForce(force);
}
