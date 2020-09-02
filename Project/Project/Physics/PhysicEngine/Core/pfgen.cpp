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

/*--------------------Spirng GENERATOR ----------*/

ParticleSpring::ParticleSpring(Particle* other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl) {}

void ParticleSpring::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// Calculate the magnitude of the force
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force.normalise();
	force *= -magnitude;
	particle->addForce(force);
}

/*--------------------Spirng Anchored GENERATOR ----------*/

ParticleAnchoredSpring::ParticleAnchoredSpring()
{
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength)
	: anchor(anchor), springConstant(springConstant), restLength(restLength) {}

void ParticleAnchoredSpring::init(Vector3* anchor, real springConstant, real restLength)
{
	ParticleAnchoredSpring::anchor = anchor;
	ParticleAnchoredSpring::springConstant = springConstant;
	ParticleAnchoredSpring::restLength = restLength;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	//Calculate the magnitudes 
	real magnitude = force.magnitude();
	if (magnitude < restLength) return;

	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force.normalise();
	force *= -magnitude;
	particle->addForce(force);
}

/*--------------------ELASTIC BUNGEE GENERATOR ----------*/

void ParticleAnchoredBungee::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	// Calculate the magnitude of the force
	real magnitude = force.magnitude();
	if (magnitude < restLength) return;

	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force.normalise();
	force *= -magnitude;
	particle->addForce(force);
}

/*------------------- BUOYANCY FORCE GENERATOR ----------*/
ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity)
: maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}
/*
f = 0 when d <= 0 (out of the water)
f = vp when d >= 1 (sumerged)
f = dvp otherwise

d = amount of the object sumerged
v = volume of the object
p = density of the water
*/
void ParticleBuoyancy::updateForce(Particle* particle, real duration)
{
	// Calculate the submersion depth
	real depth = particle->getPosition().y;

	// Check if we're out of the water
	if (depth >= waterHeight + maxDepth) return;
	Vector3 force(0, 0, 0);

	// Check if we're at maximum depth
	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->addForce(force);
		return;
	}

	// Otherwise we are partly submerged
	force.y = liquidDensity * volume *
		(depth - maxDepth - waterHeight) / (2 * maxDepth);
	particle->addForce(force);
}