#include "mmvao.h"
#include <stdio.h>
//这里必须先包含glad

#include <stdlib.h>
MMVAO::MMVAO():vao(0), ebo(0)
{
	glGenVertexArrays(1, &vao);
}


void MMVAO::addVertex3D(float* vertextData, GLsizei vertexCount,unsigned int* indics, unsigned int indicsCount, int layout)
{
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(float), vertextData, GL_STATIC_DRAW);
	//veo
	//GLuint veo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * indicsCount * sizeof(int), indics, GL_STATIC_DRAW);
	/*
		0 shader中layout的location
		3 三维坐标，可以为2,3,4
		GL_FLOAT 点参数类型上面用的是float
		3 * sizeof(float) 一个点的步长
		(GLvoid*)0 指向数据缓冲区的指针
	*/
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(layout);

	
	vboList.push_back(vbo);
	vboList.push_back(vbo);
	glBindVertexArray(0);
}

MMVAO::~MMVAO()
{
	if (vboList.size() >0)
	{
		for (GLuint vbo : vboList) 
		{
			glDeleteBuffers(1, &vbo);
		}
		vboList.clear();
	}
	if (ebo != 0)
	{
		glDeleteBuffers(1, &ebo);
	}
	if (vao != NULL)
	{
		glDeleteVertexArrays(1, &vao);
		vao = NULL;
	}
}

void MMVAO::bindVao()
{
	if (vao != NULL)
	{
		glBindVertexArray(vao);
	}
	
}