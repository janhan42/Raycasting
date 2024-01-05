	/**
 * @file Wall.cpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "SFML/Graphics/RenderWindow.hpp"
#include "Raycasting.hpp"

Wall::Wall() : thickness(8)
{

}

Wall::~Wall()
{

}

void	Wall::setStart(sf::Vector2f stratPosition)
{
	this->start = stratPosition;
}

void	Wall::setEnd(sf::Vector2f endPosition)
{
	this->end = endPosition;
}

sf::Vector2f	Wall::getStart() const
{
	return this->start;
}

sf::Vector2f	Wall::getEnd() const
{
	return this->end;
}

void	Wall::draw(sf::RenderWindow& window)
{
	sf::Vector2f direction = end - start;
	sf::Vector2f perpendicular(-direction.y, direction.x);
	perpendicular /= sqrt(perpendicular.x * perpendicular.x + perpendicular.y * perpendicular.y);
	perpendicular *= thickness / 4;

	sf::Vertex rectangle[] = {
		sf::Vertex(start - perpendicular, sf::Color::Red),
		sf::Vertex(end - perpendicular, sf::Color::Red),
		sf::Vertex(end + perpendicular, sf::Color::Red),
		sf::Vertex(start + perpendicular, sf::Color::Red)
	};
	window.draw(rectangle, 4, sf::Quads);
}
