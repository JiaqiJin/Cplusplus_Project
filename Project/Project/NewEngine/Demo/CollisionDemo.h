#pragma once

#include "Demo.h"
#include "../Defs.h"
#include "../Renderer/CollisionDemoUI.h"

class CollisionDemo : public Demo
{
public:

	CollisionDemo();
	~CollisionDemo() = default;

	virtual void Update() override;
	virtual void InputCheck() override;

	void Shoot();
};