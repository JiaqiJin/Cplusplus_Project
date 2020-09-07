#ifndef KAWAII_PWORLD_H
#define KAWAII_PWORLD_H

/*
Structure contain any number o particle mass, and their connection.
Representing the whole physically simulated world:
*/

#include "pfgen.h"
#include "plinks.h"

namespace Kawaii
{
	/*
	Track set of the particles.
	*/
	class ParticleWorld
	{
	public:
		typedef std::vector<Particle*> Particles;
		typedef std::vector<ParticleContactGenerator*> ContactGenerators;

	protected:
		/*Hold the particles*/
		Particles particles;
		/*True if the world should calculate the number of the iterations to 
		  give the contact to resolver each frame.*/
		bool calculateIterations;
		/*Hold the force generator for the particle in this world*/
		ParticleForceRegistry registry;
		/*Hold the resolve contact*/
		ParticleContactResolver resolver;
		/*Contact generator*/
		ContactGenerators contactGenerators;
		/*Hold list of the particles*/
		ParticleContact* contacts;
		/*Holds the maximum number of contacts allowed*/
		unsigned maxContacts;

	public:
		/*Create a new particle simulator that can handle up
		  to the given number of contacts per frame.
		*/
		ParticleWorld(unsigned maxContacts, unsigned iterations = 0);

		/*
		Destructor of the simulator
		*/
		~ParticleWorld();

		/*
		Initialize the worlds for a simulation frame.
		Clear the force accumulator for the particle in the world.
		Each particle have their force added for this frame.
		*/
		void startFrame();

		/*
		Calls each of the registered contact generator to report their contact.
		*/
		unsigned generateContacts();

		/*
		Integrates all the particles in this wolrd by duration.
		*/
		void integrate(real duration);

		/*
		Processes all the physics for the particle world.
		*/
		void runPhysics(real duration);

		/*Getters*/
		ContactGenerators& getContactGenerators() { return contactGenerators; }
		ParticleForceRegistry& getForceRegistry() { return registry; }
	};

	/*
	Collides them agains the ground.
	*/
	class GroundContacts : public Kawaii::ParticleContactGenerator
	{
		Kawaii::ParticleWorld::Particles* particles;
	public:
		void init(Kawaii::ParticleWorld::Particles* particles);
		virtual unsigned addContact(Kawaii::ParticleContact* contact, unsigned limit) const;
	};
}

#endif
