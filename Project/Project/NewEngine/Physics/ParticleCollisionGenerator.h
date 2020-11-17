#pragma once

#include "ParticleCollision.h"

namespace Engine
{
	class ParticleCollisionGenerator
	{
	public:
		// Add a particle collision to the list of particle collisions  
		virtual void GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit) = 0;
	};
}