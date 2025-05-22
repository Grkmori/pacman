
#include <SFML/System.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "game/Pacman.hpp"
#include "Utils.hpp"
#include "game/PacmanSettings.hpp"
#include "game/Audio.hpp"


Pacman::Pacman()
{
    std::cout << _indent(3) << "+ Called Pacman Constructor." << std::endl;
}

Pacman::~Pacman()
{
    std::cout << _indent(3) << "- Called Pacman Destructor." << std::endl;
}

// PUBLIC - INITIALIZATION
void Pacman::Initialize()
{
    Reset();
}

// PUBLIC - INTERACTIVE
void Pacman::SetupWindowStateMenu()
{
	std::cout << _indent(6) << "+ Setting pacman initial values." << std::endl;
    position = sf::Vector2f( 9.f, 5.5f );
    directionCurrent = Direction::Right;
    directionTry = Direction::None;
    directionCorrection = Direction::None;
    
    stopped = true;
    cornering = false;
    enableDraw = true;
}

// PUBLIC - SUPPORT
void Pacman::Reset()
{
	std::cout << _indent(6) << "+ Setting pacman default values." << std::endl;
    position = PACMAN_INITIAL_POSITION;
	directionCurrent = Direction::Up;
    directionTry = Direction::None;
    directionCorrection = Direction::None;
    
    stopped = true;
    cornering = false;
    enableDraw = true;
}

bool Pacman::TileCollision(std::vector<std::string>& _stringVector)
{
	bool collided = false;
	char tile = '|';

	switch (directionTry) {
		case Direction::Up: 
			tile = GetBoardTile(_stringVector, position.x, position.y - 0.51f); 
			break;
		case Direction::Down: 
			tile = GetBoardTile(_stringVector, position.x, position.y + 0.51f); 
			break;
		case Direction::Left: 
			tile = GetBoardTile(_stringVector, position.x - 0.51f, position.y); 
			break;
		case Direction::Right: 
			tile = GetBoardTile(_stringVector, position.x + 0.51f, position.y); 
			break;
		case Direction::None: break;
	}
	if (tile == '|' || tile == '-'){
        collided = true;
    }
    
	return collided;
}

bool Pacman::InTunnel()
{
	return position.x < 0.f || position.x > 27.f;
}

Direction Pacman::GetCorrection()
{
	switch (directionTry) {
		case Direction::Up: break;
		case Direction::Down:
			if (position.x - std::floor(position.x) >= 0.5f) {
				return Direction::Left;
			}
			return Direction::Right;
			break;
		case Direction::Left: break;
		case Direction::Right:
			if (position.y - std::floor(position.y) >= 0.5f) {
				return Direction::Up;
			}
			return Direction::Down;
			break;
		case Direction::None: break;
    }

	return Direction::None;
}

void Pacman::Cornering()
{
	position += DIRECTION_ADDITION[static_cast<size_t>(directionCorrection)] * SPEED_PACMAN;
	bool done = false;
	switch (directionCorrection) {
		case Direction::Up: done = (position.y - std::floor(position.y) <= 0.5f); break;
		case Direction::Down: done = (position.y - std::floor(position.y) >= 0.5f); break;
		case Direction::Left: done = (position.x - std::floor(position.x) <= 0.5f); break;
		case Direction::Right: done = (position.x - std::floor(position.x) >= 0.5f); break;
		case Direction::None: break;
	}

	if (done) {
		CenterObject();
		cornering = false;
	}
}

void Pacman::ResolveCollision()
{
	switch (directionCurrent) {
		case Direction::Up: position.y = std::floor(position.y) + 0.5f; break;
		case Direction::Down: position.y = std::floor(position.y) + 0.5f; break;
		case Direction::Left: position.x = std::floor(position.x) + 0.5f; break;
		case Direction::Right: position.x = std::floor(position.x) + 0.5f; break;
		case Direction::None: break;
	}
}

char Pacman::GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y)
{
	if (_x < 0 || _x >= static_cast<int>(_stringVector.at(_y).size())) { 
		return '/'; // For tunnel, "/" not used for anything.
	}

	return _stringVector.at(_y).at(_x);
}

void Pacman::SetTile(std::vector<std::string>& _stringVector, int _x, int _y, char _charNew)
{
	if (_x < 0 || _x >= static_cast<int>(_stringVector.at(_y).size())) {
		return; // For tunnel, "/" not used for anything.
	}

	 _stringVector.at(_y).at(_x) = _charNew;
}

// PRIVATE - SUPPORT
void Pacman::CenterObject()
{
	switch (directionCurrent) {
		case Direction::Up: break;
		case Direction::Down: position.x = std::floor(position.x) + 0.5f; break;
		case Direction::Left: break;
		case Direction::Right: position.y = std::floor(position.y) + 0.5f; break;
		case Direction::None: break;
	}
}
