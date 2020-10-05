#ifndef KAWAII_COLLISION_FINE_H
#define KAWAII_COLLISION_FINE_H
//310
#include "contacts.h"

namespace Kawaii
{
	class IntersectionTests;
	class CollisionDetector;

	/* Represents a primitive to detect collisions.
	The Primitive class holds data that any contact generator
	*/
	class CollisionPrimitive
	{
	public:
		friend class IntersectionTests;
		friend class CollisionDetector;

		RigidBody* body;
		/*represents translation and rotation only*/
		Matrix4 offset;

		/**/
		Vector3 getAxis(unsigned index) const
		{
			return transform.getAxisVector(index);
		}

	protected:
		/*The resulting transform of primitive. This is
         calculated by combining the offset of the primitive
         with the transform of the rigid body.*/
		Matrix4 transform;
	};

	/*Helper structue that contain information for the detector to 
	building it contacts data.
	*/
	struct CollisionData
	{
		/*Holds the base of the collision data: the first contact in the array.*/
		Contact* contactArray;
		/* Holds the contact array to write into. */
		Contact* contacts;
		/*Hold the maximum number of contacts the arrar can take. */
		int contactsLeft;
		/**Holds the number of contacts found so far. */
		unsigned contactCount;
		/*Holds the friction value to write into any collisions. */
		real friction;
		/*Holds the restitution value to write into any collisions. */
		real restitution;


		/*the data that the given number of contacts have been added.*/
		void addContacts(unsigned count)
		{
			contactsLeft -= count;
			contactCount += count;
			// Move the array forward
			contacts += count;
		}


	};

	/* 
	epresents a rigid body that can be treated as a sphere for collision detection.
	Two spheres are in contact if the distance
	between their centers is less than the sum of their radii.*/
	class CollisionSphere : public CollisionPrimitive
	{
	public:
		real radius;
		Vector3 position;
	};

	/*
	The collision bettwen sphere and plane, if the distance of the center of the sphere 
	is farther fro, the plane than sphere radius.
	d = p · l - l..
	l = normal vector
	l.. = offset of plane.
	*/
	class CollisionPlane
	{
	public:
		/*The plane normal*/
		Vector3 direction;
		/*The distance of the plane from the origin.*/
		real offset;
	};

	/*
	Return 4 contacts for each corner point with the half space.
	The vertices are only points, and have no radius, we simply need to check to
	see whether the sign of d is positive or negative.
	d = p · l - l..
	p · l < l..
	*/
	class CollisionBox : public CollisionPrimitive
	{
	public:
		/*Hold the half size of the box along each of local axid*/
		Vector3 halfSize;
	};

	/*
	A wrapper class that holds fast intersection tests. 
	*/
	class IntersectionTests
	{
	public:

		static bool sphereAndHalfSpace(
			const CollisionSphere& sphere,
			const CollisionPlane& plane);

		static bool sphereAndSphere(
			const CollisionSphere& one,
			const CollisionSphere& two);

		/*
		 The box is given as a transform matrix, including position, and a vector of half-sizes for the extend of the
         box along each local axis.
		*/
		static bool boxAndHalfSpace(
			const CollisionBox& box,
			const CollisionPlane& plane);
	};

	/*
	 A wrapper class that holds the fine grained collision detection routines
	*/
	class CollisionDetector
	{
	public:

		static unsigned sphereAndHalfSpace(
			const CollisionSphere& sphere,
			const CollisionPlane& plane,
			CollisionData* data);

		static unsigned sphereAndTruePlane(
			const CollisionSphere& sphere,
			const CollisionPlane& plane,
			CollisionData* data);


		static unsigned sphereAndSphere(
			const CollisionSphere& one,
			const CollisionSphere& two,
			CollisionData* data);

		static unsigned boxAndHalfSpace(
			const CollisionBox& box,
			const CollisionPlane& plane,
			CollisionData* data);

		static unsigned boxAndSphere(
			const CollisionBox& box,
			const CollisionSphere& sphere,
			CollisionData* data);

	};

}

#endif
