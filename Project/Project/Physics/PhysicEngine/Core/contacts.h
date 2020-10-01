#ifndef KAWAII_CONTACTS_H
#define KAWAII_CONTACTS_H

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
	};
}

#endif