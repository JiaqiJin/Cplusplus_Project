#include "pworld.h"

using namespace Kawaii;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
	: resolver(iterations),	maxContacts(maxContacts)
{
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);
}

ParticleWorld::~ParticleWorld()
{
	delete[] contacts;
}

/*
* Clear the force accumulator for the particle in the world.
*/
void ParticleWorld::startFrame()
{
	for (Particles::iterator p = particles.begin();
		p != particles.end(); p++)
	{
		(*p)->clearAccumulator(); // Remove all forces from the accumulator 
	}
}
/*
Generate all contact of obj in the world
*/
unsigned ParticleWorld::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;

	for (ContactGenerators::iterator g = contactGenerators.begin();
		g != contactGenerators.end(); g++)
	{
		//for each count of contact adding a contact to a obj
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0) break;
	}
	return maxContacts - limit;
}

void ParticleWorld::integrate(real duration)
{
	for (Particles::iterator p = particles.begin();
		p != particles.end(); p++)
	{
		(*p)->integrate(duration);
	}
}

void ParticleWorld::runPhysics(real duration)
{
	// apply the force generator
	registry.updateForces(duration);

	// integrate the object
	integrate(duration);

	// Generate contacts
	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		if (calculateIterations) resolver.setIterations(usedContacts * 2);
		resolver.resolveContacts(contacts, usedContacts, duration);
	}
}

void GroundContacts::init(Kawaii::ParticleWorld::Particles* particles)
{
	GroundContacts::particles = particles;
}

unsigned GroundContacts::addContact(Kawaii::ParticleContact* contact, unsigned limit) const
{
	unsigned count = 0;
	for (Kawaii::ParticleWorld::Particles::iterator p = particles->begin();
		p != particles->end(); p++)
	{
		Kawaii::real y = (*p)->getPosition().y;
		if (y < 0.0f)
		{
			contact->contactNormal = Kawaii::Vector3::UP;
			contact->particle[0] = *p;
			contact->particle[1] = NULL;
			contact->penetration = -y;
			contact->restitution = 0.2f;
			contact++;
			count++;
		}
		if (count >= limit) return count;
	}
	return count;
}