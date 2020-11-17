#pragma once

#include "../IIntegratable.h"
#include "Body.h"
#include "../math/GameTime.h"

namespace Engine
{
	class Particle : public Body
	{
	public:
		Particle(const glm::vec3& position = glm::vec3(), const float inverseMass = 0);
		~Particle();

		// Update values
		virtual void Integrate(float deltaTime) override;
		// Clear the accumulators
		virtual void ClearAccumulators() override;

	protected:
	};
}