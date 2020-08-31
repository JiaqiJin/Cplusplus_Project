#include <KawaiiDesune.h>
#include <stdio.h>

static Kawaii::Random crandom;

class Firework : public Kawaii::Particle
{
public:
	unsigned type;

	Kawaii::real age;

	bool update(Kawaii::real duration)
	{
		//update our physic state
		integrate(duration);
		age -= duration;
		return (age < 0);
	}
};

struct FireworkRule
{
	/** The type of firework that is managed by this rule. */
	unsigned type;
	/** The minimum length of the fuse. */
	Kawaii::real minAge;
	/** The maximum length of the fuse. */
	Kawaii::real maxAge;
	/** The minimum relative velocity of this firework. */
	Kawaii::Vector3 minVelocity;
	/** The maximum relative velocity of this firework. */
	Kawaii::Vector3 maxVelocity;
	/** The damping of this firework type. */
	Kawaii::real damping;
	/*the payload is the new firework type to create when this firework fuse is over*/
	struct Payload
	{
		unsigned type;
		unsigned count;
	};
	/*Number of payload for this firework type*/
	unsigned payloadCount;
	Payload* payload;

	void create(Firework* firework, const Firework* parent = NULL) const
	{
		firework->type = type;
		firework->age = crandom.randomReal(minAge, maxAge);
		Kawaii::Vector3 vel;
		if (parent)
		{
			firework->setPosition(parent->getPosition());
			vel += parent->getVelocity();
		}
		else
		{
			Kawaii::Vector3 start;
			int x = (int)crandom.randomInt(3) - 1;
			start.x = 5.0f * Kawaii::real(x);
			firework->setPosition(start);
		}
		vel += crandom.randomVector(minVelocity, maxVelocity);
		firework->setVelocity(vel);

		firework->setMass(1);

		firework->setDamping(damping);

		firework->setAcceleration(Kawaii::Vector3::GRAVITY);

		firework->clearAccumulator();
	}

};