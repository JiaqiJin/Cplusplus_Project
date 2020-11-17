#include "ForceGenerationRegistry.h"
#include "../Precompiled.h"

namespace Engine
{
	ForceGenerationRegistry::ForceGenerationRegistry()
	{}

	ForceGenerationRegistry::~ForceGenerationRegistry()
	{}

	void ForceGenerationRegistry::RemoveForceGenerator(const IParticleForceGenerator* forceGenerator)
	{
		for (auto iter = forceGenerators.begin(); iter != forceGenerators.end(); ++iter)
		{
			if (*iter == forceGenerator)
			{
				forceGenerators.erase(iter);
				break;
			}
		}
	}

	void ForceGenerationRegistry::UpdateForceGenerators(Particle* particle)
	{
		for (auto iter = forceGenerators.begin(); iter != forceGenerators.end(); ++iter)
		{
			(*iter)->UpdateForces(particle);
		}
	}
}