#include "mmprogram.h"
#include "mmshader.h"
#include <stdio.h>

#include <stdlib.h>

MMProgram::MMProgram(GLchar* vertexStr, GLchar* fragmentStr) : program(0)
{
	program = glCreateProgram();
	MMShader vertexShader = MMShader(vertexStr, MMGL_SHADER_VERTEX);
	MMShader fragmentShader = MMShader(fragmentStr, MMGL_SHADER_FRAGMENT);
	glAttachShader(program, vertexShader.getShaderId());
	glAttachShader(program, fragmentShader.getShaderId());

	glLinkProgram(program);
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char* log = (char*)malloc(logLength * sizeof(char));
			glGetProgramInfoLog(program, logLength, NULL, log);
			printf("Program Log:\n");
			printf("%s\n", log);
			free(log);
		}
	}
	
}

MMProgram::~MMProgram()
{
	if (program != 0)
	{
		glDeleteProgram(program);
		program = 0;
	}
}

int MMProgram::useProgram()
{
	glUseProgram(program);
	return 0;
}