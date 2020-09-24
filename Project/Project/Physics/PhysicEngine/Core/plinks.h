#ifndef KAWAII_PLINKS_H
#define KAWAII_PLINKS_H

#include "pcontacts.h"

namespace Kawaii
{
	/*
	Links connect 2 particle together, generating a contact if they 
	violate the constraints of thei links.
	*/
	class ParticleLink : public ParticleContactGenerator
	{
	public:
		/*Hold the pair of the particle that are conected by this link*/
		Particle* particle[2];

	protected:
		/*Return the length of the link*/
		real currentLength() const;

	public:
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};

	/*
	Cable link a pair of particles, generating a contact if the stray to far apart.
	*/
	class ParticleCable : public ParticleLink
	{
	public:
		/*Hold the maximum length of the cable*/
		real maxLength;
		/*Hold the restitution(bounciness) of the cable*/
		real restitution;
	public:
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
	};

	/*
	Rods link a pair of particles, generating a contact if they  stray too far apart or too close.
	Rods combine the behaviors of cables and collisions.
	Two objects linked by a rod can neither separate nor get closer together.
	*/
	class ParticleRod : public ParticleLink
	{
	public:
		/*Hold the length of the ROD*/
		real length;
	public:
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
	};

}

#endif