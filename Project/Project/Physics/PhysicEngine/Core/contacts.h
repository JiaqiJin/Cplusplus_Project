#ifndef KAWAII_CONTACTS_H
#define KAWAII_CONTACTS_H
//336
/*
The contact resolution system.
CONTACT DATA : 
-The collision point : the point of contact between the obj.
-The collision normal : Direction which an impact impulse will be feel between 2 obj
-The penetration depth : This is the amount that the two objects are interpenetrating.
Contact type :																							^
POINT–FACE CONTACTS : The contact normal is given by the normal of the surface at the point of contact. |
EDGE–EDGE CONTACTS : The contact normal is at right angles to the tangents of both edges.
EDGE–FACE CONTACTS : Only used with curved surfaces.The contact normal is given by the normal of the face, as before.
FACE–FACE CONTACTS : when a curved surface comes in contact with another face.The contact normal is given by the normal of the first face.

algorithms :
PRIMITIVE COLLISION ALGORITHMS

RESOLVING Interpenetration
-Linear Projection : change the position of each obj so that it is moved in the direction of the contact normal.
-Velocity-Based Resolution : take into account the linear and angular velocities of the objects in the collision. 
we could move them back to the point of first collision.calculating this point of first collision is difficult and not worth worrying about.
We need track of the vel and rotation of each obj before any collison resolution again.
-Nonlinear Projection : we use a combination of linear and angular movement to resolve the penetration.
We move 2 obj to contact normla until the are no longer interpenetrating. For each object in the collision we need to calculate the amount of 
linear motion and the amount of angular motion
*/

#include "body.h"

namespace Kawaii
{
	class ContactResolver;

	/*
	IMPULSES AND IMPULSIVE TORQUES
	the collision will generate a linear change in velocity (the impulse) and an angular change in velocity.
	An instanttaneous angular change in velicity = inmpulse torque. τ = I ¨O, for toruqe u = I`O.
	u = impulse torque, I is the inertia tensor, Δ`O = I^-1 * u.

	COLLISION IMPULSES
	To resolver the relative motion of two objs we need to calculate 4 impulse 
	-linear and angular impulses for each object. the impulse and impulsive torque for the single object.
	To calculate the impulse and impulsive force on each object :
	- Work in a set of coordinates that are relative to the contact. Create the transform matrix to convert inyo
	  and out of this new set of coordinates.
	-Wrok out the chnage in velocity of the contact point on each obj per unit impulse.
	- We will know the velocity change we want to see, invert the result of th last stage to find the impulse needed.
	-Wokr out what the separating velocity at the contact point sould be.
	-From the desired change in velocity we can calculate the impulse that must be generated.
	 split the impulse into its linear and angular components.

	Change coordinates :
	We interestng the movement of the collision point at this stage.
	First step of converting to contact coordinates is to work out the direction of each axis.

	*/
	class Contact
	{
		/*
		The contact resolver obj need acces into the contact to set and effect the contact.
		*/
		friend class ContactResolver;
	public:
		/* --- CONTACT DATA ---*/
		Vector3 contactPoint;
		Vector3 contactNormal;
		real penetration;
		/*Holds the lateral friction coefficient at the contact.*/
		real friction;
		/* Holds the normal restitution coefficient at the contact.*/
		real restitution;
		/*Hold the bodies that are involved in the contact.*/
		RigidBody* body[2];

		/**/
		void setBodyData(RigidBody* one, RigidBody* two, real friction, real restitution);

	protected:
		/*
		A transform matrix that converts the coordinates in the contact frame of reference to world coordinates.
		The columns of this matrix form an orthogonal set of vector.
		*/
		Matrix3 contactToWorld;

		/*
		Hold the closing velocity 
		*/
		Vector3 contactVelocity;

		/*
		Hold the requires change in velocity for this contact to be resolved.
		*/
		real desiredDeltaVelocity;

		/*
		 Holds the world space position of the contact point relative to 
		 centre of each body. 
		*/
		Vector3 relativeContactPosition[2];

	protected:

		/*
		Calculates internal data from state data. This is called before the resolution algorithm tries do any resolution.
		*/
		void calculateInternals(real duration);

		/*
		Reverse the contact. This involve swapping the 2 rigid bodies and reversing the contact normal.
		*/
		void swapBodies();

		/*Calculate the normal basis for the contact point, based on the primary friction direction.*/
		void calculateContactBasis();

		/*
		Claculate and set the internal value for the desired delta velocity.
		*/
		void calculateDesiredDeltaVelocity(real duration);
		/*
		 Updates the awake state of rigid bodies that are takin  place in the given contact. 
		*/
		void matchAwakeState();

		/*
		Calculate the impulse we neede to resolve this contact,
		givin that the contact has no friction.
		*/
		Vector3 calculateFrictionlessImpulse(Matrix3* inverseInertiaTensor);

		/*
		Claculate and return the velocity of the contact point by giving body
		*/
		Vector3 calculateLocalVelocity(unsigned bodyIndex, real duration);

		/*
		Persomr an inertial weight impulse.
		*/
		void applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);

		/*
		 Performs an inertia weighted penetration resolution of this contact alone.
		*/
		void applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration);
	};

	/*
	THE COLLISION RESOLUTION PIPELINE
	Collisions are generate by the collision detector based on the collision geomtry of the obj.
	The collision resolution routine has two components: 
	-a velocity resolution system.
	-a penetration resolution system.
	*/

	class ContactResolver
	{
	protected:
		/**
	  * Holds the number of iterations to perform when resolving velocity.
	  */
		unsigned velocityIterations;

		/**
		 * Holds the number of iterations to perform when resolving position.
		 */
		unsigned positionIterations;

		/**
		 * To avoid instability velocities smaller
		 * than this value are considered to be zero.
		 */
		real velocityEpsilon;

		/**
		 * To avoid instability penetrations
		 * smaller than this value are considered to be not interpenetrating.
		 */
		real positionEpsilon;

	public:
		/**
		 * Stores the number of velocity iterations used in the
		 * last call to resolve contacts.
		 */
		unsigned velocityIterationsUsed;

		/**
		 * Stores the number of position iterations used in the
		 * last call to resolve contacts.
		 */
		unsigned positionIterationsUsed;

	public:
		/*
		* Resolve the set of the contact for both penetration and velocity.
		* Contact cannot interact with each other should be passed to separate calls to resolverContacts.
		* contactArray = pointer to an array of the contacts objs.
		* numContacts = The number of contacts in the array to resolve.
		* duration  = The duration of the previous integration step.
		*/
		void resolveContacts(Contact* contactArray, unsigned numContacts, real duration);

	protected:
		/*
		* Set ups contact rdy for processing. 
		calculate the internal data for each contact and made bodies alive.
		*/
		void prepareContacts(Contact* contactArray, unsigned numContacts, real duration);

		/**
		 * Resolves the velocity issues with the given array of constraints,
		 * using the given number of iterations.
		 */
		void adjustVelocities(Contact* contactArray, unsigned numContacts, real duration);

		/**
		 * Resolves the positional issues with the given array of constraints,
		 * using the given number of iterations.
		 */
		void adjustPositions(Contact* contacts, unsigned numContacts, real duration);
	};

}

#endif