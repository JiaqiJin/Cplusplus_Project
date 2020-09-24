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
/*fdrag =-(^p.)(k1|^p.| + k2|^p.|^2)*/
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

/*--------------------Spiring GENERATOR ----------*/

ParticleSpring::ParticleSpring(Particle* other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl) {}

/*
f =-k(|d|- l0) d^
 d = end of the object spring attached.
 d = Xa - Xb
 Xa is the position of the end of the spring attached to the object 
 Xb is the position of the other end of the spring.
 |d| = magnitude of the distance between the ends of the spring.
*/
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

/*------------------- Bungee FORCE  ----------*/

ParticleBungee::ParticleBungee(Particle* other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl) {}

void ParticleBungee::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// Check if the bungee is compressed
	real magnitude = force.magnitude();
	if (magnitude <= restLength) return;

	// Calculate the magnitude of the force
	magnitude = springConstant * (restLength - magnitude);

	// Calculate the final force and apply it
	force.normalise();
	force *= -magnitude;
	particle->addForce(force);
}


/*------------------- Stiff Spring ----------*/
ParticleFakeSpring::ParticleFakeSpring(Vector3* anchor, real sc, real d)
	: anchor(anchor), springConstant(sc), damping(d) {}

/*
pt = [p0cos(ãt) + c sin(ãt)] e^(-1/2)^dt.

ã = 1/2 sqrt(4k - d^2).

c = (d/2ã) p0 + (1/ã) `p0. 

p = (pt - p0) * 1/t2 - pÿ0
*/
void ParticleFakeSpring::updateForce(Particle* particle, real duration)
{
	if (!particle->hasFiniteMass()) return;

	// Calculate the relative position of the particle to the anchor
	Vector3 position;
	particle->getPosition(&position);
	position -= *anchor;

	// Calculate the constants and check they are in bounds.
	real gamma = 0.5 * real_sqrt(4 * springConstant - damping * damping);
	if (gamma == 0) return;
	Vector3 c = position * (damping / (2.0f * gamma)) + particle->getVelocity() * (1.0f / gamma);

	// Calculate the target position
	Vector3 target = position * real_cos(gamma * duration) + c * real_sin(gamma * duration);
	target *= real_exp(-0.5f * duration * damping);

	// Calculate the resulting acceleration and therefore the force
	Vector3 accel = (target - position) * ((real)1.0 / (duration * duration)) - particle->getVelocity() * ((real)1.0 / duration);
	particle->addForce(accel * particle->getMass());

}