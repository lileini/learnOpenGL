#include "mmshader.h"
#include <stdio.h>
//这里必须先包含glad

#include <stdlib.h>

MMShader::MMShader(GLchar* shaderStr, MMGLShaderType type) : shader(0)
{
	GLenum gltype = GL_VERTEX_SHADER;
	if (type == MMGLShaderType::MMGL_SHADER_FRAGMENT)
	{
		gltype = GL_FRAGMENT_SHADER;
	}
	shader = glCreateShader(gltype);
	glShaderSource(shader, 1, &shaderStr, 0);
	glCompileShader(shader);

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		char* log = (char*)malloc(logLength * sizeof(char));
		glGetShaderInfoLog(shader, logLength, NULL, log);
		printf("%s Shader Log:\n", type == MMGLShaderType::MMGL_SHADER_FRAGMENT ? "fragment":"vertext");
		printf("%s\n", log);
		free(log);
	}
}

MMShader::~MMShader()
{
	if (shader != 0)
	{
		glDeleteShader(shader);
		shader = 0;
	}
}