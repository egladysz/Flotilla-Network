#include "ShaderElement.h"


ShaderElement::ShaderElement(const GLchar * path, GLenum shaderType)
	:shaderID{ 0 }
{
	std::string codeText;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		codeText = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: Shader File Load Failed";
	}
	const char* shaderCode = codeText.c_str();

	int successCode;
	char log[512];

	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successCode);
	if (!successCode)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, log);
		std::cout << "ERROR: Shader Compile Failed\n" << log << std::endl;
		shaderID = 0;
	}
}

ShaderElement::~ShaderElement()
{
	if (shaderID)
		glDeleteShader(shaderID);
}

glID ShaderElement::getID()
{
	return shaderID;
}
