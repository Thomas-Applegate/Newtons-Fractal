#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

static std::string read_file(const char* name)
{
	std::string ret;
	std::ifstream file(name);
	char c;
	while(file.get(c)) ret.push_back(c);
	file.close();
	return ret;
}

unsigned int create_shader()
{
	int success = 0;
	
	std::string source = read_file("vs.glsl");
	const char* sptr = source.c_str();
	
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &sptr, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		std::cerr << "Could not compile vertex shader\n";
		
		GLint maxLength = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
		
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vs, maxLength, &maxLength, &errorLog[0]);
		std::cerr << errorLog.data() << '\n';
		
		glfwTerminate();
		std::exit(1);
	}
	
	source = read_file("fs.glsl");
	sptr = source.c_str();
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &sptr, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		std::cerr << "Could not compile  fragment shader\n";
		
		GLint maxLength = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);
		
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fs, maxLength, &maxLength, &errorLog[0]);
		std::cerr << errorLog.data() << '\n';
		glfwTerminate();
		std::exit(1);
	}
	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		std::cerr << "Could not link shader\n";
		glfwTerminate();
		std::exit(1);
	}
	glDeleteShader(vs);
	glDeleteShader(fs);  
	return program;
}
