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
	layout(location = 1) in vec3 color;
	out vec3 outPos;
	out vec3 outColor;
	void main()
	{
		outPos = pos;
		outColor = color;
		//gl_Position = vec4(pos, 1);
		gl_Position = vec4(pos.x, pos.y, pos.z, 1);
	}
	
)HERE";

GLchar* fragmentShader = R"HERE(
	#version 330 core

	out vec4 rgbaColor;
	in vec3 outPos;
	in vec3 outColor;
	uniform vec4 ourColor;
	void main()
	{
		//rgbaColor = ourColor;
		rgbaColor = vec4(outColor, 1.0f);
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
        -0.5f,	-0.5f,	0.0f,//A
        0.5f,	-0.5f,	0.0f,//B
        0.5f,	0.5f,	0.0f,//C
        -0.5f,	0.5f,	0.0f,//D
    };
	//颜色值也会根据ebo位置便宜
    float colorPoints[] = {
        1.0f,	0.0f,	0.0f,
        0.0f,	1.0f,	0.0f,
        0.0f,	0.0f,	0.0f,
        0.0f,	0.0f,	1.0f,
    };

	unsigned int indics[] = {
		//三角形顺序逆时针
		0,	1,	3,//第一个三角形
		1,	2,	3 //第二个三角形
	};

	MMVAO* vao = new MMVAO();
    vao->addVertex3D(vertextPoints, std::size(vertextPoints) / 3, 0);
    vao->addVertex3D(colorPoints, std::size(colorPoints) / 3, 1);
	vao->bindIndex(indics, std::size(indics) / 3 );

	MMProgram* program = new MMProgram(vertextShader, fragmentShader);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//TODO 绘制
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		program->useProgram();
	
		

		double time = glfwGetTime();
		float greenValue = sin(time)/2.0f +0.5f;

		GLint location = glGetUniformLocation(program->getProgramId(), "ourColor");//这里是fragment shader中uniform变量名
		glUniform4f(location, 0.0f, greenValue, 0.0f, 0.0f);//rgba
		//glDrawArrays(GL_TRIANGLES, 0, std::size(vertextPoints) / 3);
		//glDrawElements(GL_TRIANGLES, std::size(indics), GL_UNSIGNED_INT, 0);
		vao->draw();

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