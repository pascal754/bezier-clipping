module;

#include <SFML/Graphics.hpp>

export module Grid;

export void drawGrids(sf::RenderWindow& window, unsigned int width, unsigned int height, unsigned int step)
{
	sf::Vertex line[2];
	line[0].color = sf::Color{ 64, 64, 64 };
	line[1].color = sf::Color{ 64, 64, 64 };
	
	
	// draw vertical lines
	line[0].position.y = 0;
	line[1].position.y = static_cast<float>(height);

	for (unsigned int i{step}; i < width; i += step)
	{
		line[0].position.x = static_cast<float>(i);
		line[1].position.x = static_cast<float>(i);
		window.draw(line, 2, sf::Lines);
	}

	// draw horizontal lines
	line[0].position.x = 0;
	line[1].position.x = static_cast<float>(width);

	for (unsigned int i{step}; i < height; i += step)
	{
		line[0].position.y = static_cast<float>(i);
		line[1].position.y = static_cast<float>(i);
		window.draw(line, 2, sf::Lines);
	}
}
