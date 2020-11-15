#include "../Precompiled.h"
#include "Application.h"

Application::Application(const char* appName, int width, int height, int fps) : window(appName, width, height, fps), 
	renderer(&window), cameraSensitivity (2.f)
{	
	inputSystem = InputSystem::GetInstance(&window);
	camera = renderer.GetCamera();

	// Setup opengl viewport
	glViewport(0, 0, width, height);

	// Set the clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	// Error callback
	glfwSetErrorCallback([](int error, const char* description) { 
		std::cout << "ERROR: " + std::string(description); });

	// Setup opengl viewport callback for when the window size changes to update the viewport
	glfwSetFramebufferSizeCallback(window.GetWindow(), [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	inputSystem->SetCursor(true);
}

void Application::Update(float deltaTime)
{
	// Camera movement
	if (inputSystem->GetKey(GLFW_KEY_W))
		camera->GetTransform().Translate(camera->GetForward() * deltaTime * cameraSensitivity);
	if (inputSystem->GetKey(GLFW_KEY_A))
		camera->GetTransform().Translate(-camera->GetSide() * deltaTime * cameraSensitivity);
	if (inputSystem->GetKey(GLFW_KEY_S))
		camera->GetTransform().Translate(-camera->GetForward() * deltaTime * cameraSensitivity);
	if (inputSystem->GetKey(GLFW_KEY_D))
		camera->GetTransform().Translate(camera->GetSide() * deltaTime * cameraSensitivity);

	// Camera rotation
	if (!inputSystem->isCursorEnabled())
	{
		const glm::vec2& offset = inputSystem->GetMouseOffset();
		if (offset.y != 0.000001f)
			camera->Pitch(offset.y * deltaTime * cameraSensitivity);
		if (offset.x != 0.000001f)
			camera->Yaw(-offset.x * deltaTime * cameraSensitivity);
	}

	// Normalize the orientation TBD: Optimize this
	//camera->GetTransform().SetOrientation(glm::normalize(camera->GetTransform().GetOrientation()));
}