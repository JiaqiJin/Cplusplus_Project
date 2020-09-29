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
//263
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
		//void insert(RigidBody* body, const BoundingVolumeClass& volume);

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
	};

	/*----------------------Methods ----------------*/
	template<class BoundingVolumeClass>
	unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
	{
		if (isLeaf() || limit == 0) return 0;
		// Get the potential contacts of one of our children with the other
		return children[0]->getPotentialContactsWith(children[1],contacts,limit);
	}

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

	/**/
	template<class BoundingVolumeClass>
	bool BVHNode<BoundingVolumeClass>::overlaps(
		const BVHNode<BoundingVolumeClass>* other) const
	{
		return volume->overlaps(other->volume);
	}

}

#endif