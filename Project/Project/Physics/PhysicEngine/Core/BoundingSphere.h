#ifndef KAWAII_COLLISION_COARSE_H
#define KAWAII_COLLISION_COARSE_H

/*
COARSE COLLISION DETECTION 
- All the collisions in the game should be contained on the list of checks.
- It should generate as small a list as possible.
- It should be as fast as possible.
Two broad approaches are used for coarse collision detection: bounding volume
hierarchies and spatial data structures.

HIERARCHIES
A bounding volume hierarchy (BVH) has each object in its bounding volume at
the leaves of a tree data structure. each of which has its own bounding volume.
If the two high-level nodes do touch, then the children of each node need to be
considered. The algorithm finally generates a list of potential contacts between objects.
*/
//286
#include <vector>
#include <cstddef>
#include "contacts.h"

namespace Kawaii
{
	struct BoundingSphere
	{
		Vector3 centre;
		real radius;

	public:
		BoundingSphere(const Vector3& centre, real radius);
		BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);

		/**/
		bool overlaps(const BoundingSphere* other) const;

		/*Reports how much this bounding sphere would have to grow 
			by incorporate the giving bounding shepre.
		*/
		real getGrowth(const BoundingSphere& other) const;

		/*
		eturns the volume of this bounding volume. 
		*/
		real getSize() const
		{
			return ((real)1.333333) * R_PI * radius * radius * radius;
		}
	};

	struct PotentialContact
	{
		/*Hold the bodies that might be in contact. */
		RigidBody* body[2];
	};

	template<class BoundingVolumeClass>
	class BVHNode
	{
	public:
		/*Holds the node immediately above us in the tree.*/
		BVHNode* parent;
		/*Hold the children node of this node*/
		BVHNode* children[2]; //Best opt 2 children per node.
		/*Hold the single boudning volume encomapassing all the descents of this node*/
		BoundingVolumeClass volume;
		/*Holds the rigid body at this node of the hierarchy.
		  Only leaf nodes can have a rigid body defined
		*/
		RigidBody* body;

		BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = NULL)
			: parent(parent), volume(volume), body(body)
		{
			children[0] = children[1] = NULL;
		}

		/*Check if the node is at the botton of the hierachy*/
		bool isLeaf() const
		{
			return (body != NULL);
		}

		/*
		Check the potential contact from this node downwards in the hierachy,
		 writing them to the given array. Returns the number of potential contact it found.
		*/
		unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const;

		/* Deletes this node, removing it first from the hierarchy, along with its associated
          rigid body and child nodes. 
		  */
		void insert(RigidBody* body, const BoundingVolumeClass& volume);

		/*
		Deletes this node, removing it first from the hierarchy, along
		with its associated rigid body and child nodes.This method deletes the node and all its children
		*/
		~BVHNode();

		//TODO

	protected:
		/*Check the potential contact between this node and the giving other.*/
		unsigned getPotentialContactsWith(
			const BVHNode<BoundingVolumeClass>* other,
			PotentialContact* contacts,
			unsigned limit) const;
		/*Check for overlapiping between nodes in the hierachy.*/
		bool overlaps(const BVHNode<BoundingVolumeClass>* other) const;

		/*For non-leaf, this method recalculates the bounding volume base on 
		   bounding volume of its children.
		*/
		void recalculateBoundingVolume(bool recurse = true);
	};

	/*----------------------Methods ----------------*/
	/*Destryuctor of BVH Tree*/
	template<class BoundingVolumeClass>
	BVHNode<BoundingVolumeClass>::~BVHNode()
	{
		// If we don't have a parent, then we ignore the sibling
		// processing
		if (parent)
		{
			// Find our sibling
			BVHNode<BoundingVolumeClass>* sibling;
			if (parent->children[0] == this) sibling = parent->children[1];
			else sibling = parent->children[0];

			//Write its data to our parent
			parent->volume = sibling->volume;
			parent->body = sibling->body;
			parent->children[0] = sibling->children[0];
			parent->children[1] = sibling->children[1];

			// Delete the sibling
			sibling->parent = NULL;
			sibling->body = NULL;
			sibling->children[0] = NULL;
			sibling->children[1] = NULL;
			delete sibling;
			// Recalculate the parent's bounding volume
			parent->recalculateBoundingVolume();
		}
		// Delete our children 
		if (children[0]) {
			children[0]->parent = NULL;
			delete children[0];
		}
		if (children[1]) {
			children[1]->parent = NULL;
			delete children[1];
		}
	}

	//Potential Contacts 
	template<class BoundingVolumeClass>
	unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
	{
		if (isLeaf() || limit == 0) return 0;
		// Get the potential contacts of one of our children with the other
		return children[0]->getPotentialContactsWith(children[1],contacts,limit);
	}
	//Potential Contacts 
	template<class BoundingVolumeClass>
	unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(
		const BVHNode<BoundingVolumeClass>* other,
		PotentialContact* contacts,
		unsigned limit) const
	{
		// Early out if we don't overlap or if we have no room
		// to report contacts
		if (!overlaps(other) || limit == 0) return 0;

		// If we're both at leaf nodes, then we have a potential contact
		if (isLeaf() && other->isLeaf())
		{
			contacts->body[0] = body;
			contacts->body[1] = other->body;
			return 1;
		}

		// Determine which node to descend into. If either is
		// a leaf, then we descend the other. If both are branches,
		// then we use the one with the largest size.
		if (other->isLeaf() ||
			(!isLeaf() && volume->getSize() >= other->volume->getSize()))
		{
			// Recurse into ourself
			unsigned count = children[0]->getPotentialContactsWith(
				other, contacts, limit
			);

			// Check we have enough slots to do the other side too
			if (limit > count) {
				return count + children[1]->getPotentialContactsWith(
					other, contacts + count, limit - count
				);
			}
			else {
				return count;
			}
		}
		else
		{
			// Recurse into the other node
			unsigned count = getPotentialContactsWith(
				other->children[0], contacts, limit
			);

			// Check we have enough slots to do the other side too
			if (limit > count) {
				return count + getPotentialContactsWith(
					other->children[1], contacts + count, limit - count
				);
			}
			else {
				return count;
			}
		}
	}

	/*
	* Insertion 
	At each node in the tree we choose the child whose bounding volume would be least expanded 
	by the addition of the new obj,The new bounding volume is calculated
	based on the current bounding volume and the new object. */
	template<class BoundingVolumeClass>
	void BVHNode<BoundingVolumeClass>::insert(RigidBody* newBody, const BoundingVolumeClass& newVolume)
	{
		// If we are a leaf, then the only option is to spawn two
		// new children and place the new body in one.
		if (isLeaf())
		{
			// Child one is a copy of us.
			children[0] = new BVHNode<BoundingVolumeClass>(this, volume, body);
			// Child two hold the new body.
			children[1] = new BVHNode<BoundingVolumeClass>(this, newVolume,newBody);
			this->body = NULL;
			recalculateBoundingVolume();
		}
		//Otherwise we nned to work out which child get to keep
		//the insert body.
		else
		{
			if (children[0]->volume.getGrowth(newVolume) <
				children[1]->volume.getGrowth(newVolume))
			{
				children[0]->insert(newBody, newVolume);
			}
			else
			{
				children[1]->insert(newBody, newVolume);
			}
		}
	}

	/*
	Recalculate new bounding volume
	*/
	template<class BoundingVolumeClass>
	void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(bool recurse)
	{
		if (isLeaf()) return;

		//use the boinding volume combining contructor
		volume = BoundingVolumeClass(
			children[0]->volume,
			children[1]->volume);
		// Recurse up the tree
		if (parent) parent->recalculateBoundingVolume();
	}


	/**/
	template<class BoundingVolumeClass>
	bool BVHNode<BoundingVolumeClass>::overlaps(
		const BVHNode<BoundingVolumeClass>* other) const
	{
		return volume->overlaps(other->volume);
	}

}

#endif