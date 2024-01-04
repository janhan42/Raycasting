/**
 * @file Player.hpp
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
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"

class Player
{
	public:
		Player(sf::Vector2f pos, float dir);
		void			setPostion(sf::Vector2f pos);
		void			setDirection(float dir);
		sf::Vector2f	getPosition();
		float			getDirection();
		void			castRays(std::vector<Wall>& walls, sf::RenderWindow& widnow);
		void			handleInput(const std::vector<Wall>& walls, sf::Event& event, sf::RenderWindow& window);
		void			draw(sf::RenderWindow& window);
		bool			isCollidingWithWall(const sf::Vector2f& newPosition, const std::vector<Wall>& walls);
		void 			handelMouse(const std::vector<Wall>& walls, sf::Event& event, sf::RenderWindow& window);
		// sf::Clock clock;
		// float lastFrameTime = 0.0f;
	private:
		sf::CircleShape	body;
		sf::Vector2f	position;
		float			direction;
		void			drawRay(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f dir);
		bool			checkLineIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2,const sf::Vector2f& p3, const sf::Vector2f& p4);
};
