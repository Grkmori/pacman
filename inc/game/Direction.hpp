
#pragma once

#include <SFML/System.hpp>

#include <vector>
#include <array>


enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right,
};

// SUPPORT
inline constexpr std::array<Direction, 5> DIRECTION_OPPOSITE = { 
	Direction::None, Direction::Down, Direction::Up, Direction::Right, Direction::Left 
};
inline const std::array<sf::Vector2f, 5> DIRECTION_ADDITION = {
	sf::Vector2f(0.f, 0.f), 
	sf::Vector2f(0.f, -1.f), sf::Vector2f(0.f, 1.f), 
	sf::Vector2f(-1.f, 0.f), sf::Vector2f(1.f, 0.f)
};
