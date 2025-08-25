#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "shader.h"

#define WIDTH 800
#define HEIGHT 600

static const float vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f,  1.0f,
	1.0f, 1.0f
};

static float scale[] = {(float)WIDTH/(float)HEIGHT, 2.0f};
static float offset[] = { 0.0f, 0.0f };

static float roots[] = {
	-1.0f, 0.0f,
	0.5f, 0.86603f,
	0.5f, -0.86603f
};

static void window_resize(GLFWwindow* window, int width, int height)
{
	scale[0] = (float)width/(float)height;
	glViewport(0, 0, width, height);
}

static void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static GLFWwindow* init()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Newton's Fractal", NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		return nullptr;
	}
	
	glViewport(0, 0, WIDTH, HEIGHT);
	
	glfwSetFramebufferSizeCallback(window, window_resize);
	return window;
}

int main(int argc, const char** argv)
{
	GLFWwindow* window = init();
	if(window == nullptr)
	{
		std::cerr << "Failed to initialize opengl\n";
		return 1;
	}
	std::cout << "opengl initialized successfully\n";
	
	//create main quad
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 
	
	//load and create shader
	unsigned int shader = create_shader();
	int uScaleLoc  = glGetUniformLocation(shader, "scale");
	int uOffsetLoc = glGetUniformLocation(shader, "offset");
	//int uRootsLoc  = glGetUniformLocation(shader, "roots"); unused for now
	
	if(uScaleLoc == -1 || uOffsetLoc == -1)
	{
		std::cerr << "Failed to get uniform location from shader\n";
		glfwTerminate();
		return 1;
	}
	
	glUseProgram(shader);
	glBindVertexArray(vao);
	while(!glfwWindowShouldClose(window)) //main program lop
	{
		glfwPollEvents();    
		process_input(window);
		
		glUniform2f(uScaleLoc, scale[0], scale[1]);
		glUniform2f(uOffsetLoc, offset[0], offset[1]);
		//glUniform2fv(uRootsLoc, 3, roots); unused for now
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	std::cout << "goodbye\n";
	return 0;
}
