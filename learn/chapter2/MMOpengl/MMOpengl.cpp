#include <stdio.h>
//这里必须先包含glad
#include "../MMGL/mmprogram.h"
#include <stdlib.h>
int main()
{
	printf("hello opengl\n");
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//大版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//小版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1280, 720, "hello opengl",NULL, NULL);
	//opengl只允许单线程绘制，多线程同时调用不被允许
	//当前函数将opengl绑定在此函数运行的线程
	glfwMakeContextCurrent(window);
	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//开始编译shader
	GLchar* shaderStr = "int main(){}";
	//MMShader* shader = new MMShader(shaderStr, MMGL_SHADER_VERTEX);
	MMProgram* program = new MMProgram(shaderStr, shaderStr);
	while (!glfwWindowShouldClose(window))
	{
		//TODO 绘制

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	//delete shader;
	delete program;
	glfwTerminate();

	return 0;
}
