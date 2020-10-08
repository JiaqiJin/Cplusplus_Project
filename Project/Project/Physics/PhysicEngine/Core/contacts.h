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
*/

#include "body.h"

namespace Kawaii
{
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

	protected:
		/*Calculate the normal basis for the contact point, based on the primary friction direction.*/
		void calculateContactBasis();
	};
}

#endif