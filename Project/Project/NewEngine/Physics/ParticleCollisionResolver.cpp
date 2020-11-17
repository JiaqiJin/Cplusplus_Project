#include "ParticleCollisionResolver.h"

namespace Engine
{
	ParticleCollisionResolver::ParticleCollisionResolver(unsigned int numIterations) : numIterations(numIterations)
	{}

	ParticleCollisionResolver::~ParticleCollisionResolver()
	{}

	/*
	1. Calculate the separating velocity of each contact, keeping track of the contact with
	the lowest (i.e., most negative) value.
	2. If the lowest separating velocity is greater than or equal to zero, then we’re done:
	exit the algorithm.
	3. Process the collision response algorithm for the contact with the lowest separating
	velocity.
	4. If we have more iterations, then return to step 1.
	*/
	void ParticleCollisionResolver::ResolveContacts(std::list<ParticleCollision>& particleCollisions)
	{
		currentIterations = 0;

		while (currentIterations != numIterations)
		{
			float maxSeparatingVelocity = 0.f;
			auto maxCollisionIter = particleCollisions.begin();
			for (auto iter = particleCollisions.begin(); iter != particleCollisions.end(); ++iter)
			{
				float sepVelocity = iter->CalculateSeparatingVelocity();
				if (sepVelocity < maxSeparatingVelocity)
				{
					maxSeparatingVelocity = sepVelocity;
					maxCollisionIter = iter;
				}
			}
			maxCollisionIter->Resolve();
			currentIterations++;
		}
	}
}