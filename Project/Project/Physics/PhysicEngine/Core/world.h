#ifndef KAWAII_WORLD_H
#define KAWAII_WORLD_H

#include "body.h"

/*
The world represente am independent simulation of the physic.
Keep track of a set of rigid bodies and update them all.
*/

namespace Kawaii
{
	class World
	{
	public:

		/*Initialize the world for a simulation frame.
		 This clear the force and torque accumulator for bodies.
		*/
		void startFrame();

		/*Process all physic in the wordl*/
		void runPhysics(real duration);

	private:
		/*Hold the single rigid body in a linked list of bodies*/
		struct BodyRegistration
		{
			RigidBody* body;
			BodyRegistration* next;
		};

		/*Hold the head of the list of registered bodies. */
		BodyRegistration* firstBody;
	};
}

#endif