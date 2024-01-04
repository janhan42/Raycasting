/**
 * @file Wall.hpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <SFML/Graphics.hpp>
#include "SFML/System/Vector2.hpp"

class Wall
{
	public:
		Wall();
		~Wall();
		void			setStart(sf::Vector2f stratPosition);
		void			setEnd(sf::Vector2f endPosition);
		sf::Vector2f	getStart() const;
		sf::Vector2f	getEnd() const;
		void			draw(sf::RenderWindow& window);
	private:
		sf::Vector2f	start;
		sf::Vector2f	end;
		float			thickness;
};
