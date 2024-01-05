#include "Raycasting.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Wall.hpp"
#include <iostream>
#include <vector>
// 인풋 컨트롤
sf::Vector2f startPosition;
sf::Vector2f g_mouse_pos;
Wall* currentSegment = nullptr;
bool Player1KeyInPut[4] = {false, false, false, false};
bool isDrawing = false;
bool isEdit = false;

void	Player1InPutHandle(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed){
		if (event.key.code == sf::Keyboard::W && !Player1KeyInPut[0]){
			Player1KeyInPut[0] = !Player1KeyInPut[0];
		}
		if (event.key.code == sf::Keyboard::S && !Player1KeyInPut[1]){
			Player1KeyInPut[1] = !Player1KeyInPut[1];
		}
		if (event.key.code == sf::Keyboard::A && !Player1KeyInPut[2]){
			Player1KeyInPut[2] = !Player1KeyInPut[2];
		}
		if (event.key.code == sf::Keyboard::D && !Player1KeyInPut[3]){
			Player1KeyInPut[3] = !Player1KeyInPut[3];
		}
	}
	if (event.type == sf::Event::KeyReleased){
		if (event.key.code == sf::Keyboard::W){
			Player1KeyInPut[0] = !Player1KeyInPut[0];
		}
		if (event.key.code == sf::Keyboard::S){
			Player1KeyInPut[1] = !Player1KeyInPut[1];
		}
		if (event.key.code == sf::Keyboard::A){
			Player1KeyInPut[2] = !Player1KeyInPut[2];
		}
		if (event.key.code == sf::Keyboard::D){
			Player1KeyInPut[3] = !Player1KeyInPut[3];
		}
	}
}

void	PressEvent(sf::Event event, std::vector<Wall>& walls, Wall*& currentSegment, bool& isDrawing)
{
	if (event.mouseButton.button == sf::Mouse::Left && !isEdit) {
		// 마우스 눌림 이벤트 처리
		startPosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
		isDrawing = true;
		currentSegment = new Wall();
		currentSegment->setStart(startPosition);
		currentSegment->setEnd(startPosition);
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

void	RelesasedEvent(sf::Event event, std::vector<Wall>& walls, Wall*& currentSegment, bool& isDrawing)
{
	if (event.mouseButton.button == sf::Mouse::Left && isDrawing && !isEdit) {
		// 마우스 뗌 이벤트 처리
		isDrawing = false;
		currentSegment->setEnd(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		walls.push_back(*currentSegment);
		delete currentSegment; // 메모리 해제
		currentSegment = nullptr;
	}

	if (event.mouseButton.button == sf::Mouse::Right && !isEdit) {
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		// 벽을 순회하면서 거리를 체크하여 삭제
		walls.erase(std::remove_if(walls.begin(), walls.end(),
			[&mousePos](const Wall& wall) {
				return distanceFromPointToLineSegment(mousePos, wall.getStart(), wall.getEnd()) < 20.0f;
			}), walls.end());
	}
}

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

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "RectangleCollision");
	window.setFramerateLimit(60);
	// setting Object
	Rectangle Player1(sf::Vector2f(10, 10), sf::Vector2f(250, 250), sf::Color::White);
	std::vector<Wall> walls;
	sf::Text text;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24); // 텍스트 크기 설정
	text.setPosition(10, 10);
	// setting window
	while (window.isOpen()){
		sf::Event event; // traking event
		while (window.pollEvent(event)){
			// 윈도우 Closed 및 Resized 관리
			if (event.type == sf::Event::Closed){
				window.close();
			} else if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			// 키 입력 이벤트 관리
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::R) {
					isEdit = !isEdit;
				}
				Player1InPutHandle(event);
			}
			if (event.type == sf::Event::KeyReleased){
				Player1InPutHandle(event);
			}
			if (event.type == sf::Event::MouseButtonPressed){
				PressEvent(event, walls, currentSegment, isDrawing);
			}
			if (event.type == sf::Event::MouseMoved) {
				MoveEvent(event, walls, currentSegment, isDrawing);
			}
			if (event.type == sf::Event::MouseButtonReleased){
				RelesasedEvent(event, walls, currentSegment, isDrawing);
			}
		}
		window.clear();
		if (!isEdit)
		{
			for (auto& wall : walls)
			{
				wall.draw(window);
			}
			if (isDrawing && currentSegment) {
				currentSegment->draw(window);
			}
			text.setString("Edit Mode\nLeft Button: Drawing\nRight Button: Delete"); // 텍스트 내용 설정
			text.setFillColor(sf::Color::Red); // 텍스트 색상을 빨간색으로 설정
		} else {
			Player1.update(window.getSize().x, window.getSize().y, 0.5f, Player1KeyInPut, walls, window);
			Player1.castRays(walls, window);
			Player1.draw(window);
			text.setString("Play Mode \n[W]:UP\n[S]:DOWN\n[A]:LEFT\n[D]:RIGHT"); // 텍스트 내용 설정
			text.setFillColor(sf::Color::Green); // 텍스트 색상을 초록색으로 설정
		}
		window.draw(text);
		window.display();
	}
}
