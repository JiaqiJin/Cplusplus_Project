#pragma once

#include "../Entity/Particle.h"
#include "IParticleForceGenerator.h"

namespace Engine
{
	class GravityForceGenerator : public IParticleForceGenerator
	{
	public:
		glm::vec3 gravitationalAcceleration; // Gravitational acceleration
		// Implementation of an update forces for a force generator will apply a gravitational force on the particle
		virtual void UpdateForces(Particle* particle) override;

		GravityForceGenerator(const glm::vec3& gravity);
		~GravityForceGenerator();
	};
}