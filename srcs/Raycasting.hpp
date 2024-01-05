#pragma once

# include <SFML/Graphics.hpp>
# include "SFML/Window/Event.hpp"
# include "Wall.hpp"
# include "Player.hpp"
# include "Ray.hpp"

#define SCREEN_W 500
#define SCREEN_H 500


float distanceFromPointToLineSegment(sf::Vector2f point, sf::Vector2f lineStart, sf::Vector2f lineEnd);
