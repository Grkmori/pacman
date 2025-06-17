
#pragma once

#include <SFML/System.hpp>

#include <string>
#include <array>
#include <unordered_map>
#include <utility> // for std::pair
#include <sstream>


enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right,
};

// SUPPORT
inline const std::array<Direction, 5> DIRECTIONS = {
    Direction::None, Direction::Up, Direction::Down, Direction::Left, Direction::Right 
};
inline const std::array<Direction, 5> DIRECTION_OPPOSITE = { 
	Direction::None, Direction::Down, Direction::Up, Direction::Right, Direction::Left 
};
inline const std::array<sf::Vector2f, 5> DIRECTION_ADDITION = {
	sf::Vector2f(0.f, 0.f), 
	sf::Vector2f(0.f, -1.f), sf::Vector2f(0.f, 1.f), 
	sf::Vector2f(-1.f, 0.f), sf::Vector2f(1.f, 0.f)
};

// SUPPORT
inline std::string DirectionToString(Direction _direction)
{
	std::stringstream ss;
	switch (_direction) {
		case Direction::None: ss << "None"; break;
		case Direction::Up: ss << "Up"; break;
		case Direction::Down: ss << "Down"; break;
		case Direction::Left: ss << "Left"; break;
		case Direction::Right: ss << "Right"; break;
		default: ss << "Unknown Direction"; break;
	}

	return ss.str();
}

inline std::pair<int, int> rotate(const std::pair<int, int>& _pair, int _angle) {
    switch (_angle) {
        case 0:   return _pair;
        case 90:  return { -_pair.second, _pair.first };   // +90° rotation.
        case -90: return { _pair.second, -_pair.first };   // -90° rotation.
        case 180: case -180: return { -_pair.first, -_pair.second }; // 180° rotation.
        default: return _pair; // Should not happen.
    }
}

inline Direction DirectionFromVector(const std::pair<int, int>& _pair) {
	if (_pair == std::pair{ 0, 0 }) return Direction::None;
    if (_pair == std::pair{ 0, -1 }) return Direction::Up;
    if (_pair == std::pair{ 0, 1 })  return Direction::Down;
    if (_pair == std::pair{ -1, 0 }) return Direction::Left;
    if (_pair == std::pair{ 1, 0 })  return Direction::Right;
    return Direction::None; // Should not happen.
}

inline Direction RelativeToAbsoluteDirection(const Direction& _facing, const std::string& _relative) {
    static const std::unordered_map<Direction, std::pair<int, int>> baseDirs = {
        {Direction::None, { 0, 0 }}, 
        {Direction::Up, { 0, -1 }}, {Direction::Down, { 0, 1 }}, 
        {Direction::Left, { -1, 0 }}, {Direction::Right, { 1, 0 }}
    };

    static const std::unordered_map<std::string, int> rotation = {
        {"None", 0}, {"Front", 0}, {"Back", 180}, {"Left", -90}, {"Right", 90}
    };

    std::pair<int, int> result = rotate(baseDirs.at(_facing), rotation.at(_relative));
    return DirectionFromVector(result);
}
