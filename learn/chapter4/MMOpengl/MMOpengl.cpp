#include <stdio.h>
//这里必须先包含glad
#include "../MMGL/mmprogram.h"
#include <stdlib.h>
#include "../MMGL/mmvao.h"

#define STRINGIZE(x) #x
#define SHADER(shader) "" STRINGIZE(shader)
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLchar* vertextShader = R"HERE(
	#version 330 core

	layout(location = 0) in vec3 pos;
	out vec3 outPos;
	void main()
	{
		outPos = pos;
		//gl_Position = vec4(pos, 1);
		gl_Position = vec4(pos.x, pos.y, pos.z, 1);
	}
	
)HERE";

GLchar* fragmentShader = R"HERE(
	#version 330 core

	out vec4 rgbaColor;
	in vec3 outPos;
	void main()
	{
		rgbaColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		//rgbaColor = vec4(outPos, 1.0f);
	}
)HERE";

int main()
{
	printf("hello opengl\n");
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//大版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//小版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "hello opengl",NULL, NULL);
	if (window == nullptr)
	{
		printf("create window fail");
		glfwTerminate();
		return -1;
	}
	//opengl只允许单线程绘制，多线程同时调用不被允许
	//当前函数将opengl绑定在此函数运行的线程
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		printf("failed to initialize to glad\n");
		glfwTerminate();
		return 0;
	}
	//开始编译shader	GLchar* shaderStr = "int main(){}";
	//MMShader* shader = new MMShader(shaderStr, MMGL_SHADER_VERTEX);

	

	float vertextPoints[] = {
		-0.5f,	-0.5f,	0.0f,//left
		0.5f,	-0.5f,	0.0f,//right
		0.0f,	0.5f,	0.0f,//top
		//1.0f,	1.0f,	0.0f,//top
	};

	MMVAO* vao = new MMVAO();
	vao->addVertex3D(vertextPoints, std::size(vertextPoints) / 3, 0);
	

	MMProgram* program = new MMProgram(vertextShader, fragmentShader);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//TODO 绘制
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		program->useProgram();
		vao->bindVao();

		glDrawArrays(GL_TRIANGLES, 0, std::size(vertextPoints) / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	//delete shader;
	delete program;
	delete vao;
	glfwTerminate();

	return 0;
}

/**
	当窗口发生改变后，重新设置绘制区域大小
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	printf("%s width:%d, height:%d\n", __FUNCTION__, width, height);
	//0,0坐标为窗口中心点右、上为正方向
	glViewport(30, 30, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}