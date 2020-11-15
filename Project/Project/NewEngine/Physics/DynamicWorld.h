#pragma once

#include "../math/GameTime.h"

namespace Engine
{
	class DynamicWorld
	{
	public:
		DynamicWorld();
		~DynamicWorld() = default;
		void Update(float dt);
	};
}