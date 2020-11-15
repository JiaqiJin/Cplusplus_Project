#include "../Precompiled.h"
#include "Window.h"
#include "UILayer.h"

Window::Window(const char* windowName, int width, int height, int fps, bool vsync) : width(width), height(height), fps(fps), vsync(vsync)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		//MTRX_ERROR("GLFW HAS NOT INITIALIZED PROPERLY!!!");
		std::cout << "GLFW HAS NOT INITIALIZED PROPERLY!!!" << std::endl;
		glfwTerminate();
		assert(false);
	}

	// Create a window and make it the current context
	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!window)
	{
		std::cout << "GLFW WINDOW HAS NOT INITIALIZED PROPERLY!!!" << std::endl;
		glfwTerminate();
		assert(false);
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "COULD NOT INITIALIZE GLAD" << std::endl;
		glfwTerminate();
		assert(false);
	}

	// Initialize UILayer of the window
	UILayer::Init(this);

	// Vsync 
	glfwSwapInterval(vsync);
}

Window::~Window()
{
	UILayer::Shutdown();

	// Terminate window
	glfwDestroyWindow(window);
}