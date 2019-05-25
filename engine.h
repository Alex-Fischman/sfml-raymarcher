#pragma once

#include <SFML/Window/Window.hpp>
#include <SFML/System/Clock.hpp>

namespace Raymarcher {
	class Engine {
		public:
			Engine(const char* title, int width, int height);
		private:
			void init();
			void loop();

			void handle(const sf::Event& event);
			void update();

			sf::Window window;

			sf::Clock frame;
			int iterations = 20;

			sf::Clock time;
			float position[3] = {0, 0, 0};
			float direction[3] = {0, 0, 1};
	};
}
