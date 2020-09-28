#include "world.h"

using namespace Kawaii;

void World::startFrame()
{
	BodyRegistration* reg = firstBody;
	while (reg)
	{
		reg->body->clearAccumulators();
		reg->body->calculateDerivedData();

		//get next registration.
		reg = reg->next;
	}
}

void World::runPhysics(real duration)
{
	BodyRegistration* reg = firstBody;
	while (reg)
	{
		// Remove all forces from the accumulator
		reg->body->integrate(duration);

		reg = reg->next;
	}

	//TODO
}