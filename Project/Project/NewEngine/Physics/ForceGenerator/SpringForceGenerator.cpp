#include "../Precompiled.h"
#include "SpringForceGenerator.h"
#include "../math/GameTime.h"

namespace Engine
{
	DefaultSpringForceGenerator::DefaultSpringForceGenerator(Particle* secondParticle, float stiffness_, float restLength_) :
		secondParticle(secondParticle),
		stiffness(stiffness_), restLength(restLength_)
	{}

	DefaultSpringForceGenerator::~DefaultSpringForceGenerator()
	{}

	void DefaultSpringForceGenerator::UpdateForces(Particle* particle)
	{
		// Get the difference in position between the 2 particles on either side of the spring
		glm::vec3 generatedForce = particle->GetPosition() - secondParticle->GetPosition();
		float mag = glm::length(generatedForce);

		// Apply Hooke's law
		mag = stiffness * abs(mag - restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}

	/*-------------Drag force genrator ----------------*/
	DragForceGenerator::DragForceGenerator(float k1, float k2) : k1(k1), k2(k2)
	{}

	DragForceGenerator::~DragForceGenerator()
	{}

	// f(drag) = -^˙p(k1|^˙p| + k2|^˙p|^2)
	// ^˙p = normalized velocity of the particle
	void DragForceGenerator::UpdateForces(Particle* particle)
	{
		// Check for infinite mass
		if (particle->GetIsInfiniteMass())
			return;

		// MIGHT NEED SOME OPTIMIZATION FOR SQRT CALLS??
		glm::vec3& velocity = particle->GetVelocity();
		float dragScalar = glm::length(particle->GetVelocity());
		dragScalar = k1 * dragScalar + k2 * dragScalar * dragScalar; // Calculate drag coefficient

		glm::vec3 dragForce = glm::normalize(velocity); // Get the direction vector of the velocity so that drag is against it
		dragForce *= -dragScalar;

		particle->AddForce(dragForce);
	}

	/*-------------------- AnchoredSpringForceGenerator -------------------------*/

	AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(glm::vec3* anchorPoint, float stiffness_, float restLength_) : 
		anchorPoint(anchorPoint),
		stiffness(stiffness_), restLength(restLength_)
	{}

	AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
	{}

	void AnchoredSpringForceGenerator::UpdateForces(Particle* particle)
	{
		// Get the difference in position between the 2 particles on either side of the spring
		glm::vec3 generatedForce = particle->GetPosition() - *anchorPoint;
		float mag = glm::length(generatedForce);

		// Check that the spring is extended since no force will be generated unless it is
		if (mag <= restLength)
			return;

		// Apply Hooke's law
		mag = stiffness * abs(mag - restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}

	
	/*------------------Bungee Spring Force Generator ---------------*/
	BungeeSpringForceGenerator::BungeeSpringForceGenerator(glm::vec3* anchorPoint, float stiffness, float restLength) :
		AnchoredSpringForceGenerator(anchorPoint, stiffness, restLength)
	{}

	BungeeSpringForceGenerator::~BungeeSpringForceGenerator()
	{}

	void BungeeSpringForceGenerator::UpdateForces(Particle* particle)
	{
		// Get the difference in position between the 2 particles on either side of the spring
		glm::vec3 generatedForce = particle->GetPosition() - *anchorPoint;
		float mag = glm::length(generatedForce);

		// Check that the spring is extended since no force will be generated unless it is
		if (mag <= restLength)
			return;

		// Apply Hooke's law
		mag = stiffness * abs(mag - restLength);

		glm::normalize(generatedForce);
		generatedForce *= -mag;
		particle->AddForce(generatedForce);
	}

	/*--------------- BUOYANCY FORCE GENERATOR------------*/

	BuoyancyForceGenerator::BuoyancyForceGenerator(float density, float volume_, float maxPaticleDepth_, float liquidHeight_) :
		liquidDensity(density) , volume(volume_), maxParticleDepth(maxPaticleDepth_), liquidHeight(liquidHeight_)
	{}

	BuoyancyForceGenerator::~BuoyancyForceGenerator()
	{}

	void BuoyancyForceGenerator::UpdateForces(Particle* particle)
	{
		// THE ASSUMPTION ATM IS THAT THE UP DIRECTION WE ARE USING IS THE Y AXIS AND THAT OUR LIQUID IS POSITIONED ACCORDINGLY 
		// IF WE WANT TO CHANGE THAT WE WOULD NEED A NEW UP DIRECTION VECTOR

		// Check if even in liquid or in need of a force first
		float currentDepth = particle->GetPosition().y;
		if (currentDepth >= liquidHeight + maxParticleDepth)
			return;

		// Apply some buoyancy force
		glm::vec3 force;

		// Partially submerged object
		if (currentDepth > liquidHeight - maxParticleDepth) // Partially submerged object 
			force.y = liquidDensity * volume * (liquidHeight + maxParticleDepth - currentDepth) / (2 * maxParticleDepth);
		else // Completely submerged object
			force.y = liquidDensity * volume;

		// Don't we need to multiply that force by gravity??
		force.y *= gravity;

		// Add the resulting force on the particle
		particle->AddForce(force);
	}

	///*----------------FakeSpringForceGenerator-----------------------------*/
	//FakeSpringForceGenerator::FakeSpringForceGenerator(glm::vec3* anchorPoint_, float springStiffness_, float springDamping_) : 
	//	anchorPoint(anchorPoint_), springStiffness(springStiffness_), springDamping(springDamping_)
	//{}

	//FakeSpringForceGenerator::~FakeSpringForceGenerator()
	//{}

	///*
	//Damped HarmonicMotion
	//damped harmonic oscillator, the differenctial equation :p = -kp -d·p [d = drag corff]
	//differential equation for the position at any time pt = [p0 cos(γt) + csin(γt)] e ^-1/2 dt
	//γ = 1/2√(4k - d^2), c = d/2γ p0 + 1/γ·p0
	//*/
	//void FakeSpringForceGenerator::UpdateForces(Particle* particle)
	//{
	//	// Check for infinite mass
	//	if (particle->GetIsInfiniteMass())
	//		return;

	//	// Get the vector from the anchor to the particle connected to this string
	//	glm::vec3 pos = particle->GetPosition();
	//	glm::vec3& velocity = particle->GetVelocity();
	//	pos *= anchorPoint;

	//	// Calculate Gamma used to calculate c (Must not be zero)
	//	float gamma = 0.5f * sqrt(4 * springStiffness - springDamping * springDamping);
	//	if (gamma == 0.0f)
	//		return;

	//	// Calculate c for the differential equation that we are using
	//	glm::vec3 c = springDamping / (2.f * gamma) * pos + velocity * 1.f / gamma;

	//	// Apply the differential equation to calculate the position
	//	glm::vec3 finalPosition = (pos * cos(gamma * GameTime::deltaTime) + 
	//		c * sin(gamma * GameTime::deltaTime)) * exp(-0.5f * springDamping * GameTime::deltaTime);

	//	// Try and get the acceleration knowing the position that it reaches 
	//	//the force f = m(:p), acce(:p) = (pt - p0) 1 / t ^ 2 - ·p0.
	//	glm::vec3 acceleration = (finalPosition - pos) * 1.0f / GameTime::deltaTime * GameTime::deltaTime - velocity * GameTime::deltaTime;

	//	// Add the new force to the particle
	//	particle->AddForce(acceleration * particle->GetMass());
	//}
}