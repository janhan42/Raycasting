/**
 * @file Raycasting.cpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Raycasting.hpp"
#include "Player.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Wall.hpp"
#include <sys/errno.h>
#include <vector>

sf::Vector2f startPosition;
bool isDrawing = false;
bool isEdit = false;
Wall* currentSegment = nullptr;
sf::Vector2f g_mouse_pos;



float distanceFromPointToLineSegment(sf::Vector2f point, sf::Vector2f lineStart, sf::Vector2f lineEnd) {
	// 선분의 벡터
	sf::Vector2f lineVec = lineEnd - lineStart;
	// 점에서 선분 시작점까지의 벡터
	sf::Vector2f pointVec = point - lineStart;
	// 선분 벡터의 제곱 길이
	float lineLenSquared = lineVec.x * lineVec.x + lineVec.y * lineVec.y;
	// 선분의 길이가 거의 0인 경우, 시작점에서의 거리 반환
	if (lineLenSquared == 0.0) return sqrt(pointVec.x * pointVec.x + pointVec.y * pointVec.y);
	// 벡터의 내적을 사용하여 점을 선분에 투영
	float t = std::max(0.0f, std::min(1.0f, (pointVec.x * lineVec.x + pointVec.y * lineVec.y) / lineLenSquared));
	// 투영된 점 (선분 위의 가장 가까운 점)
	sf::Vector2f projection = lineStart + t * lineVec;
	// 원래 점과 투영된 점 사이의 거리 반환
	return sqrt((point.x - projection.x) * (point.x - projection.x) + (point.y - projection.y) * (point.y - projection.y));
}

void	PressEvent(sf::Event event, std::vector<Wall>& walls, Wall*& currentSegment, bool& isDrawing)
{
	if (event.mouseButton.button == sf::Mouse::Left) {
		// 마우스 눌림 이벤트 처리
		startPosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
		isDrawing = true;
		currentSegment = new Wall();
		currentSegment->setStart(startPosition);
		currentSegment->setEnd(startPosition);
	}
}

void	RelesasedEvent(sf::Event event, std::vector<Wall>& walls, Wall*& currentSegment, bool& isDrawing)
{
	if (event.mouseButton.button == sf::Mouse::Left && isDrawing) {
		// 마우스 뗌 이벤트 처리
		isDrawing = false;
		currentSegment->setEnd(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		walls.push_back(*currentSegment);
		delete currentSegment; // 메모리 해제
		currentSegment = nullptr;
	}

	if (event.mouseButton.button == sf::Mouse::Right) {
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		// 벽을 순회하면서 거리를 체크하여 삭제
		walls.erase(std::remove_if(walls.begin(), walls.end(),
			[&mousePos](const Wall& wall) {
				return distanceFromPointToLineSegment(mousePos, wall.getStart(), wall.getEnd()) < 20.0f;
			}), walls.end());
	}
}

void	MoveEvent(sf::Event event, std::vector<Wall>& walls, Wall*& currentSegment, bool& isDrawing)
{
	g_mouse_pos = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
	if (isDrawing && currentSegment) {
		// 마우스 이동 이벤트 처리
		currentSegment->setEnd(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
	}
}

void setupWalls(std::vector<Wall>& walls, int screenWidth, int screenHeight, int offset) {
    // 화면의 네 귀퉁이에 대한 위치
    sf::Vector2f topLeft(offset, offset);
    sf::Vector2f topRight(screenWidth - offset, offset);
    sf::Vector2f bottomLeft(offset, screenHeight - offset);
    sf::Vector2f bottomRight(screenWidth - offset, screenHeight - offset);

    // 벽 생성 및 위치 설정
    Wall topWall;
    topWall.setStart(topLeft);
    topWall.setEnd(topRight);

    Wall rightWall;
    rightWall.setStart(topRight);
    rightWall.setEnd(bottomRight);

    Wall bottomWall;
    bottomWall.setStart(bottomRight);
    bottomWall.setEnd(bottomLeft);

    Wall leftWall;
    leftWall.setStart(bottomLeft);
    leftWall.setEnd(topLeft);

    // 벡터에 벽 추가
    walls.push_back(topWall);
    walls.push_back(rightWall);
    walls.push_back(bottomWall);
    walls.push_back(leftWall);
}

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(SCRENN_W, SCRENN_H), "Raycasting");
	window.setFramerateLimit(60);
	std::vector<Wall> walls;
	// setupWalls(walls, window.getSize().x, window.getSize().y, 15);
	Player player(sf::Vector2f(500, 500), 15);
	sf::Text text;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24); // 텍스트 크기 설정
	text.setPosition(10, 10);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			if (!isEdit)
			{
				if (event.type == sf::Event::MouseButtonPressed) {
					PressEvent(event, walls, currentSegment, isDrawing);
				}
				if (event.type == sf::Event::MouseButtonReleased) {
					RelesasedEvent(event, walls, currentSegment, isDrawing);
				}
			}
			if (event.type == sf::Event::MouseMoved) {
				MoveEvent(event, walls, currentSegment, isDrawing);
				player.handleInput(walls, event, window);
			}
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::R) {
					isEdit = !isEdit;
				}
				else {
					player.handleInput(walls, event, window);
				}
			}

		}
		window.clear();
		if (!isEdit)
		{
			for (auto& wall : walls)
			{
				wall.draw(window);
			}
			if (isDrawing && currentSegment)
			{
				currentSegment->draw(window);
			}
			text.setString("Edit Mode"); // 텍스트 내용 설정
			text.setFillColor(sf::Color::Red); // 텍스트 색상을 빨간색으로 설정
		}
		else
		{
			player.castRays(walls, window);
			player.draw(window);
			text.setString("Play Mode"); // 텍스트 내용 설정
			text.setFillColor(sf::Color::Green); // 텍스트 색상을 초록색으로 설정
		}
		window.draw(text);
		window.display();
	}
}
