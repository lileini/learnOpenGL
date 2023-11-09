#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class MMProgram
{
public:
	MMProgram(GLchar* vertexStr, GLchar* fragmentStr);
	~MMProgram();

	int useProgram();
	inline GLuint getProgramId()
	{
		return program;
	}
private:
	GLuint program;
};

