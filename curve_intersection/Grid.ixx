module;

//#include "pch.h"
#include <SFML/Graphics.hpp>

export module Grid;

import std;

export void drawGrids(sf::RenderWindow& window, unsigned int width, unsigned int height, unsigned int step)
{
  std::array<sf::Vertex, 2> line;
  line[0].color = sf::Color{ 64, 64, 64 };
  line[1].color = sf::Color{ 64, 64, 64 };


  // draw vertical lines
  line[0].position.y = 0;
  line[1].position.y = static_cast<float>(height);

  for (unsigned int i{ step }; i < width; i += step)
  {
    line[0].position.x = static_cast<float>(i);
    line[1].position.x = static_cast<float>(i);
    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
  }

  // draw horizontal lines
  line[0].position.x = 0;
  line[1].position.x = static_cast<float>(width);

  for (unsigned int i{ step }; i < height; i += step)
  {
    line[0].position.y = static_cast<float>(i);
    line[1].position.y = static_cast<float>(i);
    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
  }
}
