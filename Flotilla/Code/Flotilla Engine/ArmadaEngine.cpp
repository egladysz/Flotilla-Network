#include "ArmadaEngine.h"
#include <iostream>


ArmadaEngine::ArmadaEngine()
	:window(nullptr)
{



}

bool ArmadaEngine::setupWindow(int width, int height, const char * title)
{
	//GLFW setup
	if (window != nullptr)
	{
		glfwTerminate();
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	//for resize event callback function
	//glfwSetFramebufferSizeCallback(window, [CALLBACK FUNCTION NAME]);
	
	//GLAD setup
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization failed" << std::endl;
		return false;
	}

	glViewport(0, 0, width, height);

	return true;
}


ArmadaEngine::~ArmadaEngine()
{
}

GLFWwindow * ArmadaEngine::currentWindow()
{
	return window;
}
