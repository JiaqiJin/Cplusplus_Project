#include "fgen.h"

using namespace Kawaii;

Gravity::Gravity(const Vector3& gravity)
	: gravity(gravity) {}

void Gravity::updateForce(RigidBody* body, real duration)
{
	// Check that we do not have infinite mass
	if (!body->hasFiniteMass()) return;
	// Apply the mass-scaled force to the body
	body->addForce(gravity * body->getMass());
}

Spring::Spring(const Vector3& localConnectionPt,
	RigidBody* other,
	const Vector3& otherConnectionPt,
	real springConstant,
	real restLength)
	: connectionPoint(localConnectionPt),
	otherConnectionPoint(otherConnectionPt),
	other(other),
	springConstant(springConstant),
	restLength(restLength) {}

void Spring::updateForce(RigidBody* body, real duration)
{
	// Calculate the two ends in world space
	Vector3 lws = body->getPointInWorldSpace(connectionPoint);
	Vector3 ows = other->getPointInWorldSpace(otherConnectionPoint);

	// Calculate the vector of the spring
	Vector3 force = lws - ows;

	// Calculate the magnitude of the force
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force.normalise();
	force *= -magnitude;
	body->addForceAtPoint(force, lws);
}