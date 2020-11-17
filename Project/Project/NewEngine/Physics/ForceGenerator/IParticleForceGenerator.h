#pragma once

#include "../Entity/Particle.h"

namespace Engine
{
	class IParticleForceGenerator
	{
	public:
		virtual void UpdateForces(Particle* particle) = 0;
	};
}