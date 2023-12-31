﻿#include <stdio.h>
//这里必须先包含glad
#include "../MMGL/mmprogram.h"
#include <stdlib.h>
#include "../MMGL/mmvao.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	layout(location = 2) in vec2 aTexCoord;
	out vec3 outPos;
	out vec3 outColor;
	out vec2 texCoord;
	uniform mat4 transform;
	void main()
	{
		outPos = pos;
		outColor = color;
		gl_Position = transform * vec4(pos, 1);
		//gl_Position = vec4(pos.x, pos.y, pos.z, 1);
		texCoord = aTexCoord;
	}
	
)HERE";

GLchar* fragmentShader = R"HERE(
	#version 330 core

	out vec4 rgbaColor;

	in vec3 outPos;
	in vec3 outColor;
	in vec2 texCoord;

	uniform vec4 ourColor;
	uniform sampler2D texture0;
	uniform sampler2D texture1;

	void main()
	{
		//rgbaColor = ourColor;
		//rgbaColor = vec4(outColor, 1.0f);
		//rgbaColor = vec4(outPos, 1.0f);
		//vec2 newCoord = vec2(texCoord.x, 1 - texCoord.y);//纹理图片放进来是翻转的，显示需要对y坐标进行翻转


		//rgbaColor = mix(texture(texture0, texCoord), texture(texture1, newCoord),0.5);//用于颜色混合
		
		//两张图覆盖混合贴图，和用于实际两张图片合并算法
		vec2 newTexCoord = vec2(texCoord.x, texCoord.y);
		vec4 bgColor = texture(texture0, newTexCoord);
		vec4 fgColor = texture(texture1, newTexCoord);
		
		rgbaColor.rgb = fgColor.rgb * fgColor.a + bgColor.rgb*(1-fgColor.a)*bgColor.a;
		rgbaColor.a = max(fgColor.a ,ceil(1.0 - bgColor.a)*bgColor.a);
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
    //颜色值也会根据ebo位置变换
    float colorPoints[] = {
        1.0f,	0.0f,	0.0f, //0
        0.0f,	1.0f,	0.0f, //1
        0.0f,	0.0f,	0.0f, //2
        0.0f,	0.0f,	1.0f, //3
    };

    unsigned int indics[] = {
        //三角形顺序逆时针
        0,	1,	3,//第一个三角形
        1,	2,	3 //第二个三角形
    };
	//这里坐标实际1/2.0f进行放置
	float textrueCoords[] = {
		0.0f,	0.0f,
		1.0f,	0.0f,
        1.0f,	1.0f,
        0.0f,	1.0f
	};

	GLuint texture0;
	//texture0
	glGenTextures(1, &texture0);
	//激活TEXTURE0后面绑定的texture都是TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);//翻转图片避免贴图加载时图片不正确
	unsigned char *data = stbi_load("../../../../../../resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		int format = GL_RGB;
		if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,data );
		glGenerateMipmap(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
	}
	else
	{
		printf("failed to load texture\n");
	}
	stbi_image_free(data);
	//解绑texture
	glBindTexture(GL_TEXTURE_2D, 0);

    //texture1
    GLuint texture1;
    glGenTextures(1, &texture1);
    //激活TEXTURE1后面绑定的texture都是TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


    //int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data1 = stbi_load("../../../../../../resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data1)
    {
        int format = GL_RGB;
        if (nrChannels == 4)
        {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
        //glActiveTexture(GL_TEXTURE0);
    }
    else
    {
        printf("failed to load texture1\n");
    }
    stbi_image_free(data1);
    //解绑texture
    glBindTexture(GL_TEXTURE_2D, 0);

	MMVAO* vao = new MMVAO();
	vao->addVertex3D(vertextPoints, std::size(vertextPoints),3, 0);
    vao->addVertex3D(colorPoints, std::size(colorPoints),3, 1);
    vao->addVertex3D(textrueCoords, std::size(textrueCoords) ,2, 2);
	vao->bindIndex(indics, std::size(indics) );


	MMProgram* program = new MMProgram(vertextShader, fragmentShader);
	program->useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glUniform1i(glGetUniformLocation(program->getProgramId(), "texture0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(program->getProgramId(), "texture1"), 1);
    glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(0.2f, 0.2f, 0.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 1.0f));
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(program->getProgramId(), "transform"), 1, GL_FALSE, glm::value_ptr(trans));
	glUseProgram(0);

	glm::mat4(1.0f);
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
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}