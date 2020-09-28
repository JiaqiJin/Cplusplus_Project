#ifndef KAWAII_FGEN_H
#define KAWAII_FGEN_H

#include "body.h"
#include "pfgen.h"
#include <vector>

namespace Kawaii
{
	class ForceGenerator
	{
	public:
		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

	class Gravity : public ForceGenerator
	{
	public:
		Gravity(const Vector3& gravity);
		virtual void updateForce(RigidBody* body, real duration);

	private:
		Vector3 gravity;
	};

	/*A force generator that applies a Spring force.
	We need know where the spring attach to each obj.
	*/
	class Spring : public ForceGenerator
	{
	public:
		Spring(const Vector3& localConnectionPt,
			RigidBody* other,
			const Vector3& otherConnectionPt,
			real springConstant,
			real restLength);
		virtual void updateForce(RigidBody* body, real duration);
	private:
		/*Connection of the spring.*/
		Vector3 connectionPoint;
		Vector3 otherConnectionPoint;
		RigidBody* other;
		real springConstant;
		real restLength;
	};

	/*A force generator that applies an aerodynamic force.
	  Fa = A * Vw
	  Fa = resulting force.
	  A = Aerodinamic tensor
	  Vw = velocity of the air.
	*/
	class Aero : public ForceGenerator
	{
	protected:
		/*Hold the aerodynamic tenfor for the surface in bdy space.*/
		Matrix3 tensor;
		/*Hold the relative position of the aeordynamic surface in the bodie coord*/
		Vector3 position;
		/*Holds a pointer to a vector containing the windspeed of the enviroment.*/
		const Vector3* windspeed;

	public:
		Aero(const Matrix3& tensor, const Vector3& position,
			const Vector3* windspeed);

		virtual void updateForce(RigidBody* body, real duration);

	protected:
		/*Create the new aerodymanic force generator with the given property.*/
		void updateForceFromTensor(RigidBody* body, real duration,
			const Matrix3& tensor);
	};

	/*A force generator with a control aerodynamic surface.
		This requiere 3 inertial tensor, for 2 extremes and the resting pos of 
		the control surface.
	*/
	class AeroControl : public Aero
	{
	public:
		AeroControl(const Matrix3& base,
			const Matrix3& min, const Matrix3& max,
			const Vector3& position, const Vector3* windspeed);

		/*Sets the conbtrol position of this control.*/
		void setControl(real value);

		virtual void updateForce(RigidBody* body, real duration);

	private:
		//calculates the final aerodynamic tensor for the current
		Matrix3 getTensor();
	protected:
		Matrix3 maxTensor;
		Matrix3 minTensor;
		/*The current position of the control for this surface.(-1,0,1)min max tensor value used*/
		real controlSetting;
	};

	/*A force generator to apply a buoyant force to a rigid body.
	The center of buoyancy is from the center of mass, 
	the more torque will be generated and the better the boat will be at righting itself. 
	*/
	class Buoyancy : public ForceGenerator
	{
	private:
		/*The max submersion depth of the obj before it generates its maximum buoyancy force*/
		real maxDepth;
		/*volume of the obj*/
		real volume;
		/*The height of the water plane above y=0.*/
		real waterHeight;
		/*The density of the liquid. Pure water has a density of 1000kg per cubic meter.*/
		real liquidDensity;
		/* The centre of buoyancy of the rigid body, in body coordinates.*/
		Vector3 centreOfBuoyancy;
	public:
		Buoyancy(const Vector3& cOfB,
			real maxDepth, real volume, real waterHeight,
			real liquidDensity = 1000.0f);

		virtual void updateForce(RigidBody* body, real duration);

	};

}

#endif