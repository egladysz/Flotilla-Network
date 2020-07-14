#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Terminology.h"

class ShaderElement
{
private:
	glID shaderID;
public:
	ShaderElement(const GLchar* path, GLenum shaderType);
	~ShaderElement();

	glID getID();
};

