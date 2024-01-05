#include "Player.hpp"
#include "SFML/Graphics/Color.hpp"
#include <iostream>
#include <valarray>

Rectangle::Rectangle(sf::Vector2f size, sf::Vector2f pos, sf::Color color) :
Velocity(0,0),
accelation(0, 0),
gracity(0.0f, 9.8f),
direction(0)
{
	body.setSize(size);
	body.setPosition(pos);
	body.setOrigin(size.x / 2, size.y / 2);
	body.setFillColor(color);
}

Rectangle::~Rectangle() {}

void	Rectangle::setSize(sf::Vector2f size){
	body.setSize(size);
}
void	Rectangle::setPosition(sf::Vector2f pos){
	body.setPosition(pos);
}
void	Rectangle::setVelocity(sf::Vector2f vel){
	this->Velocity = vel;
}
sf::Vector2f Rectangle::getSize(){
	return this->size;
}
sf::Vector2f Rectangle::getVelocity(){
	return this->Velocity;
}
sf::Vector2f Rectangle::getAccelation(){
	return this->accelation;
}

void Rectangle::update(float screenWidth, float screenHeight, float deltaTime, bool inPut[4], std::vector<Wall>& walls, sf::RenderWindow& window)
{
	if (inPut[0]) { // W key
		accelation.y = -1.0f; // Move up
	} else if (inPut[1]) { // S key
		accelation.y = 1.0f; // Move down
	} else {
		if (Velocity.y != 0) {
			if (Velocity.y > 0)
				accelation.y = -1.0f;
			else if (Velocity.y < 0)
				accelation.y = 1.0f;
		} else {
			accelation.y = 0.0f;
			Velocity.y = 0.0f;
		}
	}
	if (inPut[2]) { // A key
		accelation.x = -1.0f; // Move left
	} else if (inPut[3]) { // D key
		accelation.x = 1.0f; // Move right
	} else {
		if (Velocity.x != 0){
			if (Velocity.x > 0)
				accelation.x = -1.0f;
			else if (Velocity.x < 0)
				accelation.x = 1.0f;
		} else {
			accelation.x = 0.0f;
			Velocity.x = 0.0f;
		}
	}
	Velocity += accelation * deltaTime;

	sf::Vector2f newPosition = body.getPosition() + Velocity * deltaTime;
	sf::Vector2f size = body.getSize();

	if (newPosition.x - size.x / 2 < 0) {
		newPosition.x = size.x / 2;
		Velocity.x = 0;
	}
	// Check right boundary
	else if (newPosition.x + size.x / 2 > screenWidth) {
		newPosition.x = screenWidth - size.x / 2;
		Velocity.x = 0; // Stop the movement
	}
	// Check top boundary
	if (newPosition.y - size.y / 2 < 0) {
		newPosition.y = size.y / 2;
		Velocity.y = 0; // Stop the movement
	}
	// Check bottom boundary
	else if (newPosition.y + size.y / 2 > screenHeight) {
		newPosition.y = screenHeight - size.y / 2;
		Velocity.y = 0; // Stop the movement
	}
	// Velocity.y += gracity.y * deltaTime; // Gravity
	sf::Vector2f topLeft = newPosition - sf::Vector2f(size.x / 2, size.y / 2);
	sf::Vector2f topRight = sf::Vector2f(newPosition.x + size.x / 2, newPosition.y - size.y / 2);
	sf::Vector2f bottomLeft = sf::Vector2f(newPosition.x - size.x / 2, newPosition.y + size.y / 2);
	sf::Vector2f bottomRight = newPosition + sf::Vector2f(size.x / 2, size.y / 2);
	for (const auto& wall : walls) {
		if (checkLineIntersection(topLeft, topRight, wall.getStart(), wall.getEnd()) ||
			checkLineIntersection(topRight, bottomRight, wall.getStart(), wall.getEnd()) ||
			checkLineIntersection(bottomRight, bottomLeft, wall.getStart(), wall.getEnd()) ||
			checkLineIntersection(bottomLeft, topLeft, wall.getStart(), wall.getEnd())) {
			// 충돌 발생, 이동 취소
			Velocity.x = 0.0f;
			Velocity.y = 0.0f;
			return;
		}
	}
	body.setPosition(newPosition);
}

bool Rectangle::isCollidingWithWall(const sf::Vector2f& newPosition, const std::vector<Wall>& walls) {
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
bool Rectangle::checkLineIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2,
									const sf::Vector2f& p3, const sf::Vector2f& p4) {
	float denominator = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);
	if (denominator == 0) return false; // 선분이 평행한 경우

	float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
	float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

	// ua와 ub가 [0, 1] 범위 내에 있으면 선분이 교차
	return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}

void	Rectangle::draw(sf::RenderWindow &window){
	window.draw(body);
}

void	Rectangle::castRays(std::vector<Wall>& walls, sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos); // 스크린 좌표를 월드 좌표로 변환

	// 마우스 위치를 향한 방향 벡터 계산
	sf::Vector2f mouseDirection = mouseWorldPos - body.getPosition();
	float length = std::sqrt(mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y);
	direction = std::atan2(mouseDirection.y, mouseDirection.x); // 각도를 라디안으로 계산
	const float rayCount = 2500.0f; // 레이의 수
	const float totalAngle = 35.0f * (M_PI / 180.0f); // 전체 각도
	const float angleStep = totalAngle / rayCount; // 각 레이 사이의 각도
	float currentAngle = direction - angleStep * (rayCount / 2); // 시작 각도
	for (int i = 0; i < rayCount; i++) {
		sf::Vector2f radDir(cos(currentAngle), sin(currentAngle));
		Ray ray(mouseDirection /= length); // 레이 생성
		ray.reset(body.getPosition(), currentAngle); // 레이의 끝점 초기화
		for (const auto& wall : walls) {
			ray.calc_hit(wall.getStart(), wall.getEnd()); // 각 벽에 대한 교차점 계산
		}
		drawRay(window, body.getPosition(), ray.getEnd()); // 레이 그리기
		ray.drawRayEnd(window, ray);
		currentAngle += angleStep;
	}
}

void Rectangle::drawRay(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end) {
	sf::Vector2f direction = end - start;
	sf::Vector2f perpendicular(-direction.y, direction.x);
	float thickness = 5.0f;  // 레이의 두께
	perpendicular = perpendicular / std::sqrt(perpendicular.x * perpendicular.x + perpendicular.y * perpendicular.y) * thickness;

	sf::Vertex quad[] = {
		sf::Vertex(start - perpendicular, sf::Color(150, 150, 150, 1)),
		sf::Vertex(start + perpendicular, sf::Color(150, 150, 150, 1)),
		sf::Vertex(end + perpendicular, sf::Color(150, 150, 150, 1)),
		sf::Vertex(end - perpendicular, sf::Color(150, 150, 150, 1))
	};
	window.draw(quad, 4, sf::Quads);
}
