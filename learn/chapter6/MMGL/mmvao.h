#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


class MMVAO
{
public:
	MMVAO();
	~MMVAO();

	/*
		data 定点数组
		vertexCount 定点数量
		layout shader中layout的location值

	*/
	void addVertex3D(float* vertextData, GLsizei vertexCount, unsigned int* indics, unsigned int indicsCount, int layout);
	void bindVao();

private:
	GLuint vao;
	std::vector<GLuint> vboList;
	GLuint ebo;
};
