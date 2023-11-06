#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum MMGLShaderType
{
	MMGL_SHADER_VERTEX = 1,
	MMGL_SHADER_FRAGMENT = 2
	
};


class MMShader
{
public:
	MMShader(GLchar* shaderStr, MMGLShaderType type);
	~MMShader();

	inline GLuint getShaderId() 
	{
		return shader;
	}
private:
	GLuint shader;
};

