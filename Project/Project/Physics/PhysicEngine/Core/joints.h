#ifndef KAWAII_JOINTS_H
#define KAWAII_JOINTS_H

#include "contacts.h"

/*
Joins class link 2 rigid bodies together and make sure they dont separate.
The contact is keeping two point together. The friction in joins will be infinite to avoid 
slipping out of aligment.
*/

namespace Kawaii
{
	class Joint : public ContactGenerator
	{
	public:
		/*Holds the two rigid bodies that are connected by this joint.*/
		RigidBody* body[2];
		/*Hold ther relative location of the connection for each body*/
		Vector3 position[2];
		/*Hold the maximum displacement at the joints before joint is considered violated*/
		real error;
		/*Generate the contact reqeuires to restore the joints if has violated*/
		unsigned addContact(Contact* contact, unsigned limit) const;
		/*Configured the joint in one go*/
		void set(RigidBody* a, const Vector3& a_pos,
				 RigidBody* b, const Vector3& b_pos, real error);
	};
}

#endif