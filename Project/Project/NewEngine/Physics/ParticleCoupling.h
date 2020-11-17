#pragma once

#include "ParticleCollisionGenerator.h"
#include "Entity/Particle.h"
#include "ParticleCollision.h"

namespace Engine
{
	class ParticleCoupling : public ParticleCollisionGenerator
	{
	public:
		Particle* particles[2]; // The 2 particles to be coupled

		ParticleCoupling();
		~ParticleCoupling();

		// Gives us the length of the cable / rod that connects the particles squared
		inline float GetCurrentLengthSqr() const { return glm::length2(particles[0]->GetPosition() - particles[1]->GetPosition()); }
		virtual void GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit) = 0;
	};

	class ParticleCableCoupling : public ParticleCoupling
	{
	public:
		ParticleCableCoupling(float maxLengthSqr, float restitution);
		~ParticleCableCoupling();

		float maxLengthSqr; // The square of the max length of the cable used to check for generating a contact
		float restitution; // The bounciness of the cable

		virtual void GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit);
	};

	class ParticleRodCoupling : public ParticleCoupling
	{
	public:
		float lengthSqr; // Length of the rod

		ParticleRodCoupling(float lengthSqr);
		~ParticleRodCoupling();

		// Implementation of the generation of a contact with a rod
		virtual void GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit);
	};

}