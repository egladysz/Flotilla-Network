#include "Shader.h"

Shader::Shader()
	:ID{ 0 }
{
	ID = glCreateProgram();
}

void Shader::prime()
{
	int successCode;
	char log[512];

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &successCode);
	if (!successCode)
	{
		glGetProgramInfoLog(ID, 512, NULL, log);
		std::cout << "ERROR: Shader Linking Failed\n" << log << std::endl;
		ID = 0;
	}
}


void Shader::use()
{
	glUseProgram(ID);
}

void Shader::addElement(ShaderElement se)
{
	glAttachShader(ID, se.getID());
}



void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec4(const std::string & name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

Shader::~Shader()
{
}
