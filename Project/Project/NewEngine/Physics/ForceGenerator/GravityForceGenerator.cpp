#include "GravityForceGenerator.h"

namespace Engine
{
	GravityForceGenerator::GravityForceGenerator(const glm::vec3& gravity_) : gravitationalAcceleration(gravity_){}

	GravityForceGenerator::~GravityForceGenerator()
	{}

	void GravityForceGenerator::UpdateForces(Particle* particle)
	{
		// Check for infinite mass
		if (particle->GetIsInfiniteMass())
			return;

		// Add gravitational force to the particle according to f = ma
		particle->AddForce(gravitationalAcceleration * particle->GetMass());
	}
}