
#pragma once

#include <SFML/System.hpp>

#include <string>
#include <vector>

#include "game/GameSettings.hpp"
#include "game/States.hpp"
#include "game/Direction.hpp"


// GENERAL
// enum class GhostName 
// {
//     Blinky, // Red.
//     Pinky, // Pink.
//     Inky, // Blue.
//     Clyde // Orange.
// };

inline const float SPEED_GHOST = SPEED_FULL * 0.75f;
inline const float SPEED_FRIGHT = SPEED_FULL * 0.5f;

//inline const float SPEED_DEFAULT = 0.15f; not used.
inline const float SPEED_FRIGHTENED = 0.05f; // std 0.1
inline const float SPEED_GO_HOME = 0.2f; // std 0.4f 
inline const float SPEED_HOME_BASE = 0.05f;


// INITIALIZATION
inline const std::vector<std::string> GHOSTS_NAMES = {
    "Blinky", "Pinky", "Inky", "Clyde" 
};
inline const std::vector<GhostState> GHOSTS_STATES = { 
    GhostState::Corner, GhostState::HomeBase, GhostState::HomeBase, GhostState::HomeBase 
};
inline const std::vector<sf::Vector2f> GHOSTS_POSITIONS = { 
    { 14.f, 11.5f }, { 14.f, 14.5f }, { 12.f, 14.5f }, { 16.f, 14.5f } 
};
inline const std::vector<Direction> GHOSTS_DIRECTIONS = { 
    Direction::Left, Direction::Up, Direction::Down, Direction::Down 
};
inline const std::vector<float> GHOSTS_SPEED = { 
    SPEED_GHOST, SPEED_HOME_BASE, SPEED_HOME_BASE, SPEED_HOME_BASE 
};
inline const std::vector<bool> GHOST_IS_IN_HOUSE = {
    false, true, true, true 
};
inline const std::vector<bool> GHOST_ENABLE_DRAW = {
    true, true, true, true 
};

// OTHER
inline const std::vector<Direction> ENTER_DIRECTION = { 
    Direction::Up, Direction::Up, Direction::Left, Direction::Right 
};
inline const std::vector<float> STARTING_X = { 14.f, 14.f, 12.f, 16.f };
inline const std::vector<int> PELLET_COUNTER = { 0, 60, 90, 120 }; // std 0, 0, 30, 60
inline const std::vector<int> GLOBAL_PELLET_LIMIT = { 0, 15, 30, 45 }; // std 0, 7, 15, 32
inline const std::vector<sf::Vector2f> CORNERS = { 
    { 31.f, 0.f }, { 0.f, 0.f }, { 31.f, 31.f }, { 0.f, 31.f } 
};

// SUPPORT
inline int GhostNameToInt(std::string _name)
{
	int n;
    if (_name == "Blinky") {
        n = 0;
    } else if (_name == "Pinky") {
        n = 1;
    } else if (_name == "Inky") {
        n = 2;
    } else if (_name == "Clyde") {
        n = 3;
    }

	return n;
}
