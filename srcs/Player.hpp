#pragma once
#include "Raycasting.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "Wall.hpp"

class Rectangle
{
	public:
		Rectangle(sf::Vector2f size, sf::Vector2f pos, sf::Color color);
		~Rectangle();
		void				setSize(sf::Vector2f size);
		void				setPosition(sf::Vector2f pos);
		void				setVelocity(sf::Vector2f vel);
		sf::Vector2f		getSize();
		sf::Vector2f		getVelocity();
		sf::Vector2f		getAccelation();
		void				update(float screenWidth, float screenHeight, float deltaTime, bool inPut[4], std::vector<Wall>& walls, sf::RenderWindow& window);
		bool				isCollidingWithWall(const sf::Vector2f& newPosition, const std::vector<Wall>& walls);
		bool				checkLineIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, const sf::Vector2f& p4);
		void				draw(sf::RenderWindow& window);
		void				castRays(std::vector<Wall>& walls, sf::RenderWindow& window);
		void				drawRay(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);

	private:
		sf::RectangleShape	body;
		sf::Vector2f		size;
		sf::Vector2f		position;
		sf::Vector2f		Velocity;
		sf::Vector2f		accelation;
		sf::Vector2f		gracity;
		float				direction;
		bool 				isJump = false;
};
