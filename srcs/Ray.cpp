/**
 * @file Ray.cpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Raycasting.hpp"
#include <iostream>

Ray::Ray(sf::Vector2f direction)
{
	// Set end point relative to mouse cursor
	// Add arbitrary length
	m_relative_end = direction * 3000.0f;
}

// Reset end-point of Ray
void Ray::reset(sf::Vector2f playerPosition, float playerDirection, float length) {
	// 레이의 방향을 플레이어의 방향으로 설정
	while (playerDirection < 0) playerDirection += 2 * M_PI;
	while (playerDirection >= 2 * M_PI) playerDirection -= 2 * M_PI;
	// 레이의 끝점을 플레이어의 위치에서 시작하여 레이의 방향으로 설정
	sf::Vector2f rayDirection = sf::Vector2f(cos(playerDirection), sin(playerDirection));
	m_end = playerPosition + rayDirection * length;
	Player_pos = playerPosition;
}

const sf::Vector2f& Ray::getEnd() const{
	return m_end;
}

// Calculates intersection-point two lines
// Used for getting intersection between Ray and wall
// More or less black-box code
void Ray::calc_hit(Vector2f p3, Vector2f p4)
{
	const Vector2f p1 = Player_pos;
	const Vector2f p2 = m_end;

	// Calculates denominator of equations
	const float den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

	if (den == 0)
		return;

	const float t =  ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
	const float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / den;

	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1)
	{
		// Gets intersection point
		m_end.x = p1.x + t * (p2.x - p1.x);
		m_end.y = p1.y + t * (p2.y - p1.y);
	}
}

void	Ray::setEndPoint(const sf::Vector2f& point)
{
	endPoint = point;
}

const sf::Vector2f& Ray::getEndPoint() const
{
	return m_end;
}

void Ray::drawRayEnd(sf::RenderWindow& window, const Ray& ray, sf::Vector2f playerPosition, float playerDirection, float length) {
	sf::Vector2f rayDirection = sf::Vector2f(cos(playerDirection), sin(playerDirection));
	sf::Vector2f endPoint = playerPosition + rayDirection * length;

	// Ray의 실제 길이 계산
	float actualLength = std::sqrt(std::pow(ray.getEndPoint().x - playerPosition.x, 2) + std::pow(ray.getEndPoint().y - playerPosition.y, 2));
	// 설정한 length와 다를 경우에만 원을 그림
	if (std::abs(actualLength - length) > 0.1) { //오차 범위
		sf::CircleShape circle(2); // 반지름
		circle.setPosition(ray.getEndPoint());
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setFillColor(sf::Color::White); // 색상 설정
		window.draw(circle);
	}
}
