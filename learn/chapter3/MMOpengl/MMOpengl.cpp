#include <stdio.h>
//这里必须先包含glad
#include "../MMGL/mmprogram.h"
#include <stdlib.h>

#define STRINGIZE(x) #x
#define SHADER(shader) "" STRINGIZE(shader)

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

	GLchar* vertextShader = SHADER(
		#version 330\n

		layout(location = 0) in vec3 pos;
		out vec3 outPos;
		void main() 
		{
			outPos = pos;
			gl_Position = vec4(pos, 1);
		}
	);
	printf("%s\n", vertextShader);
	GLchar* fragmentShader = SHADER(
		#version 330\n
		out vec4 rgbaColor;
		in vec3 outPos;
		void main()
		{
			rgbaColor = vec4(outPos, 1.0);
		}
	);
	MMProgram* program = new MMProgram(vertextShader, fragmentShader);
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
