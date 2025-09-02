#include <iostream>
#include <thread>
#include <chrono>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader.h"

#define WIDTH 800
#define HEIGHT 600

using namespace std::chrono_literals;

static const float vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f,  1.0f,
	1.0f, 1.0f
};

static float viewport[] = {(float)WIDTH, (float)HEIGHT, 0.5f};
static float offset[] = { 0.0f, 0.0f };

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
	glfwSwapInterval(0);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		return nullptr;
	}
	
	glViewport(0, 0, WIDTH, HEIGHT);
	
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		viewport[0] = float(width);
		viewport[1] = float(height);
		glViewport(0, 0, width, height);
	});
	
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	});
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
	ImGui_ImplGlfw_InitForOpenGL(window, true);// Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	
	return window;
}

int program_loop(GLFWwindow* window)
{
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
	int uViewportLoc  = glGetUniformLocation(shader, "viewport");
	int uOffsetLoc = glGetUniformLocation(shader, "offset");
	int uRootsLoc  = glGetUniformLocation(shader, "roots");
	int uItersLoc  = glGetUniformLocation(shader, "iterations");
	
	if(uViewportLoc == -1 || uOffsetLoc == -1 || uRootsLoc == -1 || uItersLoc == -1)
	{
		std::cerr << "Failed to get uniform location from shader\n";
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteProgram(shader);
		return 1;
	}
	
	//uniform variable local to this function
	float roots[] = {
		1.0f, 0.0f,
		-0.5f, 0.86603f,
		-0.5f, -0.86603f
	};
	
	int numIters = 50;
	
	double previousTime = glfwGetTime();

	while(!glfwWindowShouldClose(window)) //main program lop
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		glfwPollEvents();
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		//render fractal
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glUniform3f(uViewportLoc, viewport[0], viewport[1], viewport[2]);
		glUniform2f(uOffsetLoc, offset[0], offset[1]);
		glUniform2fv(uRootsLoc, 3, roots);
		glUniform1i(uItersLoc, numIters);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		//render imgui
		ImGui::Begin("Fractal Controls");
		ImGui::Text("FPS: %f", 1.0f/deltaTime);
		ImGui::SliderFloat2("root 1", roots, -8.0, 8.0, "%.5f");
		ImGui::SliderFloat2("root 2", roots+2, -8.0, 8.0, "%.5f");
		ImGui::SliderFloat2("root 3", roots+4, -8.0, 8.0, "%.5f");
		if(ImGui::Button("Reset Roots"))
		{
			roots[0] = 1.0f;
			roots[1] = 0.0f;
			roots[2] = -0.5f;
			roots[3] = 0.86603f;
			roots[4] = -0.5f;
			roots[5] = -0.86603f;
		}
		ImGui::InputInt("iterations", &numIters, 1, 10);
		if(numIters < 0) numIters = 0;
		if(numIters > 2000) numIters = 2000;
		ImGui::SliderFloat("zoom", viewport+2, 0.03125, 1024.0, "%.5f");
		ImGui::SliderFloat2("view offset", offset, -16.0, 16.0, "%.5f");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(1ms);
	}
	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader);
	return 0;
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
	
	int ret = program_loop(window);
	
	//clean up and shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	std::cout << "goodbye\n";
	return ret;
}
