/**
 * @file Raycasting.hpp
 * @author jang hun han (wkdgns720@naver.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

# include <SFML/Graphics.hpp>
# include "SFML/Graphics/RenderWindow.hpp"
# include "SFML/Window/Event.hpp"
# include "SFML/Window/VideoMode.hpp"
# include "SFML/System/Vector2.hpp"
# include "Wall.hpp"
# include "Player.hpp"
# include "Ray.hpp"

# define SCRENN_W 1980
# define SCRENN_H 1020
extern sf::Vector2f g_mouse_pos;
extern bool isRotatingLeft;
extern bool isRotatingRight;
