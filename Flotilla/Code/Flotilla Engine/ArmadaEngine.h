#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Terminology.h"
class ArmadaEngine
{
public:
	ArmadaEngine();
	bool setupWindow(int width, int height, const char* title);
	~ArmadaEngine();
	GLFWwindow* currentWindow();
private:
	GLFWwindow* window;
};

