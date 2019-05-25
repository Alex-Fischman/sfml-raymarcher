#include "engine.h"

#include "render.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>

const float PI = std::acos(-1);

Raymarcher::Engine::Engine(const char* title, int width, int height): window(
		sf::VideoMode(width, height),
		title,
		sf::Style::Default,
		sf::ContextSettings(0, 0, 0 /* antialiasing */, 4, 4, sf::ContextSettings::Attribute::Core, false)
), frame(), time() {
	init();
	loop();
}

void Raymarcher::Engine::init() {
	glewExperimental = true;
	glewInit();

	window.setActive(true);

	Render::init();

	time.restart();
	frame.restart();
}

void Raymarcher::Engine::loop() {
	while (window.isOpen()) {
		update();
		Render::draw(window);

		sf::Event event;
		while (window.pollEvent(event)) handle(event);
	}
}

void Raymarcher::Engine::handle(const sf::Event& event) {
	switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
			glViewport(0, 0, event.size.width, event.size.height);
			break;
	}
}

void Raymarcher::Engine::update() {
	float fps = 1 / frame.getElapsedTime().asSeconds();
	frame.restart();
	if (fps < 95) iterations -= 5;
	if (fps > 105) iterations += 5;

	position[2] = -5;
	direction[0] = std::cos(time.getElapsedTime().asSeconds()) / 5;
	direction[1] = std::sin(time.getElapsedTime().asSeconds()) / 5;

	glUniform2f(0, window.getSize().x, window.getSize().y);
	glUniform1i(1, iterations);
	glUniform1f(2, 1.0 / window.getSize().x);
	glUniform1f(3, 100);
	float camera[8] = {position[0], position[1], position[2], 0.0, direction[0], direction[1], direction[2], 0.0};
	glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), camera, GL_STATIC_DRAW);
}
