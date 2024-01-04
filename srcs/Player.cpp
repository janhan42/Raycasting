/**
 * @file Player.cpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Player.hpp"
#include "Raycasting.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <iostream>
#include <cmath>

Player::Player(sf::Vector2f pos, float dir)
{
	body.setRadius(10.0f);
	body.setPosition(pos);
	body.setOrigin(body.getRadius(), body.getRadius());
	body.setFillColor(sf::Color::White);
}

void	Player::setPostion(sf::Vector2f pos)
{
	this->position = pos;
}

void	Player::setDirection(float dir)
{
	this->direction = dir;
}

sf::Vector2f	Player::getPosition()
{
	return this->position;
}

float	Player::getDirection()
{
	return this->direction;
}

void Player::handleInput(const std::vector<Wall>& walls, sf::Event& event, sf::RenderWindow& window) {
	const float moveSpeed = 5.0f;
	const float rotationSpeed = 0.01f;

	sf::Vector2f movement(0.0f, 0.0f);
	std::cout << "direction : "<< direction << std::endl;
	if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f newPosition(mousePosition.x, mousePosition.y);
        body.setPosition(newPosition); // 플레이어의 setPosition 함수를 호출하여 위치 업데이트
    }

	const float maxRotationSpeed = 30.f; // 최대 회전 속도
	float currentRotationSpeed = 0.0f;    // 현재 회전 속도
	float rotationAcceleration = 1.0f;
	float rotationDeceleration = 1.0f;

	if (event.key.code == sf::Keyboard::A) {
		currentRotationSpeed -= rotationAcceleration; // 회전 가속
		if (currentRotationSpeed < -maxRotationSpeed) {
			currentRotationSpeed = -maxRotationSpeed;
		}
	} else if (event.key.code == sf::Keyboard::D) {
		currentRotationSpeed += rotationAcceleration; // 회전 가속
		if (currentRotationSpeed > maxRotationSpeed) {
			currentRotationSpeed = maxRotationSpeed;
		}
	} else {
		// 회전 속도를 서서히 줄입니다.
		if (currentRotationSpeed < 0) {
			currentRotationSpeed += rotationDeceleration;
			if (currentRotationSpeed > 0) currentRotationSpeed = 0;
		} else if (currentRotationSpeed > 0) {
			currentRotationSpeed -= rotationDeceleration;
			if (currentRotationSpeed < 0) currentRotationSpeed = 0;
		}
	}

	sf::Vector2f oldPosition = body.getPosition();
	sf::Vector2f newPosition = oldPosition + movement;
	if (!isCollidingWithWall(newPosition, walls)) {
		oldPosition = newPosition;
		body.setPosition(oldPosition);
		body.setOrigin(body.getRadius(), body.getRadius());
	}
	body.setPosition(oldPosition);
	fmod(direction, 720.0f);
	body.setRotation(direction);
}

bool Player::isCollidingWithWall(const sf::Vector2f& newPosition, const std::vector<Wall>& walls) {
	sf::Vector2f oldPosition = this->position; // 플레이어의 이전 위치

	for (const auto& wall : walls) {
		if (checkLineIntersection(oldPosition, newPosition, wall.getStart(), wall.getEnd())) {
			std::cout << "Intersection: TURE" << std::endl;
			return true; // 충돌 발생
		}
	}
	std::cout << "Intersection: false" << std::endl;
	return false; // 충돌 없음
}

// 두 선분의 교차 여부를 확인하는 함수
bool Player::checkLineIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2,
									const sf::Vector2f& p3, const sf::Vector2f& p4) {
	float denominator = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);
	if (denominator == 0) return false; // 선분이 평행한 경우

	float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
	float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

	// ua와 ub가 [0, 1] 범위 내에 있으면 선분이 교차
	return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}

void	Player::castRays(std::vector<Wall>& walls, sf::RenderWindow& window)
{
	const float rayCount = 1440.0f; // 레이의 수
	const float totalAngle = 1440.0f * (M_PI / 180.0f); // 전체 각도
	const float angleStep = totalAngle / rayCount; // 각 레이 사이의 각도
	float currentAngle = direction - angleStep * (rayCount / 2); // 시작 각도
	for (int i = 0; i < rayCount; i++) {
		sf::Vector2f radDir(cos(currentAngle), sin(currentAngle));
		Ray ray(radDir); // 레이 생성
		ray.reset(body.getPosition(), direction + currentAngle); // 레이의 끝점 초기화
		for (const auto& wall : walls) {
			ray.calc_hit(wall.getStart(), wall.getEnd()); // 각 벽에 대한 교차점 계산
		}
		drawRay(window, body.getPosition(), ray.getEnd()); // 레이 그리기
		ray.drawRayEnd(window, ray);
		currentAngle += angleStep;
	}

}
void Player::drawRay(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end) {
    sf::Vector2f direction = end - start;
    sf::Vector2f perpendicular(-direction.y, direction.x);
    float thickness = 1.0f;  // 레이의 두께
    perpendicular = perpendicular / std::sqrt(perpendicular.x * perpendicular.x + perpendicular.y * perpendicular.y) * thickness;

    sf::Vertex quad[] = {
        sf::Vertex(start - perpendicular, sf::Color(240, 240, 240, 10)),
        sf::Vertex(start + perpendicular, sf::Color(240, 240, 240, 10)),
        sf::Vertex(end + perpendicular, sf::Color(240, 240, 240, 10)),
        sf::Vertex(end - perpendicular, sf::Color(240, 240, 240, 10))
    };


    window.draw(quad, 4, sf::Quads);
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(body);
}
