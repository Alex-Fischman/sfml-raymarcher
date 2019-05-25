#include <SFML/Window/Window.hpp>

#include <string>

namespace Raymarcher {
	namespace Render {
		void init();
		void draw(sf::Window& window);
		
		namespace Check {
			int program(int program);
			int shader(int shader);
		}
	}

	std::string file(const char* filename);
}