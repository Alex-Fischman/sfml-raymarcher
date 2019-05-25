#include "render.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

void Raymarcher::Render::init() {
	//Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexSource = "#version 440 core\n" + file("vertex.glsl");
	const char* vertexSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	if (Check::shader(vertexShader) == 1) return;

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentSource = "#version 440 core\n" + file("sdfs.glsl") + file("fragment.glsl");
	const char* fragmentSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	if (Check::shader(fragmentShader) == 1) return;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	if (Check::program(program) == 1) return;

	glUseProgram(program);

	//VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//VBO
	GLuint vbo;
	static const float screen[8] = {-1, 1, 1, 1, 1, -1, -1, -1};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), screen, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);	
	glEnableVertexAttribArray(0);

	//Camera
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
}

void Raymarcher::Render::draw(sf::Window& window) {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	window.display();
}

int Raymarcher::Render::Check::program(int program) {
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char log[length];
		glGetProgramInfoLog(program, length, NULL, &log[0]);

		std::cerr << log << "\n";
		return 1;
	}
	return 0;
}

int Raymarcher::Render::Check::shader(int shader) {
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		char log[length];
		glGetShaderInfoLog(shader, length, NULL, &log[0]);

		std::cerr << log << "\n";
		return 1;
	}
	return 0;
}

std::string Raymarcher::file(const char* filename) {
	std::string source;
	std::ifstream fileStream(filename, std::ios::in);
	std::stringstream stringStream;
	stringStream << fileStream.rdbuf();
	source = stringStream.str();
	fileStream.close();
	return source;
}
