#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderElement.h"
class Shader
{
public:

	unsigned int ID;

	Shader();

	void use();

	void addElement(ShaderElement);
	void prime();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	~Shader();
};

