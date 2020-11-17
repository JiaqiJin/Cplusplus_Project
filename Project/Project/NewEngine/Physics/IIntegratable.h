#pragma once

namespace Engine
{
	class IIntegratable
	{
	public:
		virtual void Integrate(float deltaTime) = 0;	// Call a physics update
	};
}