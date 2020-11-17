#pragma once

#include "Entity/Particle.h"

/*
Closing velocity
Vc = -(·Pa - ·Pb)·(^(Pa - Pb)), Vc` = -cVc  
Impulse g = m(·p)
Total change in velocity (·p`) = ·p + (1/m)g
*/
namespace Engine
{
	class ParticleCollision
	{
	public:
		ParticleCollision(Particle* particle1, Particle* particle2, float restitution, glm::vec3& normal);
		~ParticleCollision();

		Particle* particles[2]; // The 2 particles that are colliding
		float restitution; // Restitution constance that allows us to know the ratio of how much separating velocity we will use
		float penetration; // The depth of penetration between the 2 objects
		glm::vec3 collisionNormal; // The normal of the collision used to calculate the separating velocity

		void Resolve(); // Allows us to resolve a collision
		float CalculateSeparatingVelocity() const; // Calculate the separating velocity between 2 particles
		inline void SetCollisionNormal(glm::vec3& normal) { collisionNormal = normal; } // Mostly used for collision between particle and static object
	private:
		void ResolveVelocity(); // Modify velocity according to separating velocity, restitution etc...
		void ResolveInterpenetration(); // Resolves collision penetration
	};
}