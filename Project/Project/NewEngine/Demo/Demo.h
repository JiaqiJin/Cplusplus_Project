#pragma once

#include "../Renderer/Input/InputSystem.h"
#include "../Renderer/Window.h"
#include "../Renderer/Application.h"
#include "../math/GameTime.h"
#include "../Renderer/SimpleRenderer.h"
#include "../Physics/DynamicWorld.h"

class Demo
{
public:
	Application application;
	bool cursor;
	std::unordered_set<Engine::Transform*> transformsToRender;
	int mesh;

	Demo(const char* appName = "DEMO APPLICATION", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT);
	virtual ~Demo() = default;

	Engine::DynamicWorld world;

	void BaseInputCheck();
	virtual void InputCheck() = 0;
	virtual void Update();

	void Run();
};