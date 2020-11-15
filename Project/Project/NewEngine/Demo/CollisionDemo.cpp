#include "CollisionDemo.h"
#include "../Precompiled.h"

CollisionDemo::CollisionDemo() : Demo("COLLISION DEMO", 1366, 768)
{
	application.inputSystem->ToggleCursor();
}


void CollisionDemo::Update()
{
	// Create the UI 
	CollisionDemoUI ui = CollisionDemoUI("Collision Demo", glm::vec2(300, 300));

	while (!application.window.ShouldClose())
	{
		// Update the demo
		Demo::Update();
	}
}

void CollisionDemo::InputCheck()
{
	if (application.inputSystem->GetKeyDown(GLFW_KEY_SPACE))
		Shoot();
}

void CollisionDemo::Shoot()
{

}