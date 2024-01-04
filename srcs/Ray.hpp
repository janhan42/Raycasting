/**
 * @file Ray.hpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Raycasting.hpp"
#include "SFML/System/Vector2.hpp"

using namespace sf;

class Ray
{
public:
	Vector2f m_end;

	Ray() {};
	Ray(sf::Vector2f direction);
	void calc_hit(Vector2f wall1, Vector2f wall2);
	void reset(sf::Vector2f playerPosition, float playerDirection);
	const sf::Vector2f& getEnd() const;
	sf::Vector2f endPoint;
	void drawRayEnd(sf::RenderWindow& window, const Ray& ray);
	const sf::Vector2f& getEndPoint() const;
private:
	void	setEndPoint(const sf::Vector2f& point);
	Vector2f m_relative_end;
	sf::Vector2f Player_pos;
};
