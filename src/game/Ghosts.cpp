

#include <SFML/System.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "game/Ghosts.hpp"
#include "Utils.hpp"


Ghosts::Ghosts(std::string _name)
: name(_name)
{
    std::cout << _indent(3) << "+ Called Ghosts Constructor. " << 
        "Ghost name: " << name << "." << std::endl;

    Initialize();
}

Ghosts::~Ghosts()
{
    std::cout << _indent(3) << "- Called Ghosts Destructor. " << 
        "Ghost name: " << name << "." << std::endl;
}

// PUBLIC - INITIALIZATION
void Ghosts::Initialize()
{
    Reset();
}

// PUBLIC - INTERACTIVE
void Ghosts::SetupWindowStateMenu()
{
    float n = static_cast<float>(GhostNameToInt(this->name));
    this->state = GhostState::Chase;
    this->position = { 9.f, 8.5f + n * 3.f };
    this->directionCurrent = Direction::Right;
    this->updateDirection = false;
    this->speed = SPEED_GHOST;
    this->isInHouse = false;
    this->enableDraw = true;
    this->pelletsCounter = 0;
    std::cout << _indent(6) << "+ Setting ghost: " << this->name << " initial values. State: " << 
        std::to_string(static_cast<size_t>(this->state)) << std::endl;
}

// PUBLIC - SUPPORT
void Ghosts::Reset()
{
    int n = GhostNameToInt(this->name);
    this->state = GHOSTS_STATES[n];
    this->position = GHOSTS_POSITIONS[n];
    this->directionCurrent = GHOSTS_DIRECTIONS[n];
    this->updateDirection = false;
    this->speed = GHOSTS_SPEED[n];
    this->isInHouse = GHOST_IS_IN_HOUSE[n];
    this->enableDraw = GHOST_ENABLE_DRAW[n];
    this->pelletsCounter = 0;
    std::cout << _indent(6) << "+ Setting ghost: " << this->name << " default values. State: " <<
        std::to_string(static_cast<size_t>(this->state)) << std::endl;
}

void Ghosts::UpdateGhostState(std::vector<std::string>& _stringVector, GhostState _state)
{
	if (this->state == GhostState::GoHome) {
        return;
    }
	switch (_state) {
        case GhostState::LeaveHome:
            if (this->isInHouse) {
                this->state = _state;
            }
            std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                std::to_string(static_cast<size_t>(this->state)) << std::endl;
            break;
        case GhostState::Frightened:
            if (!this->isInHouse) {
                this->directionCurrent = this->GetOppositeTile(_stringVector);
                this->speed = SPEED_FRIGHT;
                this->state = _state;
                std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                    std::to_string(static_cast<size_t>(this->state)) << std::endl;
            }
            break;
        case GhostState::GoHome:  // Technically it will never get here.
            if (!this->isInHouse) {
                this->speed = SPEED_GO_HOME;
                this->state = _state;
                std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                    std::to_string(static_cast<size_t>(this->state)) << std::endl;
            }
            break;
        default:
            if (!this->isInHouse) {
                // Ghost reverses direction whenever changing states,
                // except when coming from frightened.
                if (this->state != GhostState::Frightened) {
                    this->directionCurrent = this->GetOppositeTile(_stringVector);
                }
                this->speed = SPEED_GHOST;
                this->state = _state;
                std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                    std::to_string(static_cast<size_t>(this->state)) << std::endl;
            }
            break;
	}
}

void Ghosts::UpdateGhosts(bool _boolCounter, int _intPelletCounter, int _intWaveCounter,
    Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer, 
    std::vector<std::string>& _stringVector
) {
    sf::Vector2f prevPosition = this->position;
    this->position += DIRECTION_ADDITION[static_cast<size_t>(this->directionCurrent)] * this->speed;
    
    if (this->isInHouse) {
        this->HouseUpdate(_boolCounter, _intPelletCounter, _intWaveCounter);
    }
    // If ghost position is in the middle of the tile, didn't update last turn, 
    // and isn't in the tunnel, do waypoint calculations, else do nothing.
    else if (!this->updateDirection &&
        InMiddleTile(this->position, prevPosition, this->directionCurrent) &&
        !InTunnel(this->position)
    ) {
        this->UpdateDirection(
            GetAvailableSquares(_stringVector, this->position, this->directionCurrent, false),
            _pacman, _ghostsContainer
        );
        this->updateDirection = true;
    } else {
        this->updateDirection = false;
    }

    // Blech, at least it terminates quickly.
    if (this->state == GhostState::GoHome && this->PassedEntrence()) {
        this->state = GhostState::EnterHome;
        this->isInHouse = true;
        this->directionCurrent = Direction::Down;
        this->position.x = 14;
        std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
            std::to_string(static_cast<size_t>(this->state)) << std::endl;
    }

    // Tunneling.
    if (this->position.x < -1) {
        this->position.x += 29;
    } else if (this->position.x >= 29) {
        this->position.x -= 29;
    }
}

GhostState Ghosts::GetGlobalTarget(int _intWaveCounter)
{
    if (_intWaveCounter >= 7) {
        return GhostState::Chase;
    }
    
    if (_intWaveCounter % 2) {
        return GhostState::Chase;
    } else {
        return GhostState::Corner;
    }
}

// PRIVATE - SUPPORT
Direction Ghosts::GetOppositeTile(std::vector<std::string>& _stringVector)
{
	// This will be the case 99% of times.
	if (!this->TileCollision(_stringVector, 
            this->position + DIRECTION_ADDITION[static_cast<size_t>(
                DIRECTION_OPPOSITE[static_cast<size_t>(this->directionCurrent)]
            )] * 0.9f
        )
    ) {
        return DIRECTION_OPPOSITE[static_cast<size_t>(this->directionCurrent)];
    }

    // Last case scenario, don't reverse.
	auto squares = GetAvailableSquares(_stringVector, this->position, this->directionCurrent);
	if (!squares.empty()) {
        return squares.at(0);
    }
		
	return this->directionCurrent; 
}

std::vector<Direction> Ghosts::GetAvailableSquares(std::vector<std::string>& _stringVector, 
    sf::Vector2f _vector2f, Direction _direction, bool _boolHomeTile
) { // TODO: This should be cached/hardcoded, but its performant enough.
	std::vector<Direction> squares;

	// Up is picked before left if the distances are tied etc.
	if (!TileCollision(_stringVector, _vector2f + sf::Vector2f{ 0.9f, 0.f }, _boolHomeTile)) {
        squares.push_back(Direction::Right);
    }
	if (!TileCollision(_stringVector, _vector2f + sf::Vector2f{ 0.f, 0.9f }, _boolHomeTile)) {
        squares.push_back(Direction::Down);
    }
	if (!TileCollision(_stringVector, _vector2f + sf::Vector2f{ -0.9f, 0.f }, _boolHomeTile)) {
        squares.push_back(Direction::Left);
    }
	if (!TileCollision(_stringVector, _vector2f + sf::Vector2f{ 0.f, -0.9f }, _boolHomeTile)) {
        squares.push_back(Direction::Up);
    }
	for (unsigned i = 0; i < squares.size(); i++) {
		if (squares[i] == DIRECTION_OPPOSITE[static_cast<size_t>(_direction)]) {
            squares.erase(squares.begin() + i);
        }   
	}

	return squares;
}

bool Ghosts::TileCollision(std::vector<std::string>& _stringVector, 
    sf::Vector2f _vector2f, bool _boolHomeTile
) {
	bool collided = false;
	char tile = GetBoardTile(_stringVector, _vector2f.x, _vector2f.y);

	if (tile == '|' || tile == '/') {
        collided = true;
    }
	if (!_boolHomeTile && tile == '-') {
        collided = true;
    }

	return collided;
}

char Ghosts::GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y)
{
	if (_x < 0 || _x >= static_cast<int>(_stringVector.at(_y).size())) { 
		return '/'; // For tunnel, "/" not used for anything.
	}

	return _stringVector.at(_y).at(_x);
}

void Ghosts::HouseUpdate(bool _boolCounter, int _intPelletCounter, int _intWaveCounter)
{
	switch (this->state) {
        case GhostState::EnterHome:
            if (this->position.y >= 15.f) {
                this->directionCurrent = ENTER_DIRECTION[GhostNameToInt(this->name)];
                this->position.y = 15.f;
                if (FComp(this->position.x, STARTING_X[GhostNameToInt(this->name)]) || 
                    (this->directionCurrent == Direction::Left && 
                        this->position.x <= STARTING_X[GhostNameToInt(this->name)]
                    ) || 
                    (this->directionCurrent == Direction::Right && 
                        this->position.x >= STARTING_X[GhostNameToInt(this->name)]
                    )
                ) {
                    this->state = GhostState::HomeBase;
                    this->speed = SPEED_HOME_BASE;
                    std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                        std::to_string(static_cast<size_t>(this->state)) << std::endl;
                }
            }
            break;
        case GhostState::LeaveHome:
            if (FComp(this->position.x, 14.f) || 
                (this->directionCurrent == Direction::Left && this->position.x <= 14.f) || 
                (this->directionCurrent == Direction::Right && this->position.x >= 14.f)
            ) {
                this->directionCurrent = Direction::Up;
                this->position.x = 14.f;
                if (this->position.y <= 11.5f) {
                    this->speed = SPEED_GHOST;
                    this->state = GetGlobalTarget(_intWaveCounter);
                    this->position.y = 11.5f;
                    this->isInHouse = false;
                    this->directionCurrent = Direction::Left;
                    std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                        std::to_string(static_cast<size_t>(this->state)) << std::endl;
                }
            }
            else {
                this->directionCurrent = DIRECTION_OPPOSITE[static_cast<size_t>(
                    ENTER_DIRECTION[GhostNameToInt(this->name)]
                )];
            }
            break;
        case GhostState::HomeBase:
            this->speed = SPEED_HOME_BASE;

            if (_boolCounter) { // Check timer if its time to leave.
                if (_intPelletCounter >= GLOBAL_PELLET_LIMIT[GhostNameToInt(this->name)]) {
                    this->state = GhostState::LeaveHome;
                    std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                        std::to_string(static_cast<size_t>(this->state)) << std::endl;
                }       
            } else if (this->pelletsCounter >= PELLET_COUNTER[GhostNameToInt(this->name)]) {
                this->state = GhostState::LeaveHome;
                std::cout << _indent(9) << "+ Ghost " << this->name << " new state: " <<
                    std::to_string(static_cast<size_t>(this->state)) << std::endl;
            }

            if (this->position.y <= 14.f) {
                this->position.y = 14.f;
                this->directionCurrent = Direction::Down;
            } else if (this->position.y >= 15.f) {
                this->position.y = 15.f;
                this->directionCurrent = Direction::Up;
            }
            break;
        default: 
            break;
	}
}

bool Ghosts::InMiddleTile(sf::Vector2f _vector2fPosition, sf::Vector2f _vector2fPrevious, 
    Direction _direction
) {
	if ((int)_vector2fPosition.x != (int)_vector2fPrevious.x || 
        (int)_vector2fPosition.y != (int)_vector2fPrevious.y
    ) {
        return false;
    }
		
	float x = _vector2fPosition.x - (int)_vector2fPosition.x;
	float y = _vector2fPosition.y - (int)_vector2fPosition.y;

	float px = _vector2fPrevious.x - (int)_vector2fPrevious.x;
	float py = _vector2fPrevious.y - (int)_vector2fPrevious.y;

	switch (_direction) {
        case Direction::Up: 
            return std::min(y, py) <= 0.5f && std::max(y, py) >= 0.5f;
            break;
        case Direction::Down:
            return std::min(y, py) <= 0.5f && std::max(y, py) >= 0.5f;
            break;
        case Direction::Left: 
            return std::min(y, py) <= 0.5f && std::max(y, py) >= 0.5f;
            break;
        case Direction::Right:
            return std::min(x, px) <= 0.5f && std::max(x, px) >= 0.5f;
            break;
        case Direction::None: break;
	}

	return false;
}

void Ghosts::UpdateDirection(const std::vector<Direction>& _vectorDirection,
    Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer
) {
	sf::Vector2f target;
	bool updateDirection = false;
	if (_vectorDirection.size() == 1) {
		this->directionCurrent = _vectorDirection.at(0);
	}

	switch (this->state)	{
        case GhostState::Frightened:
            this->directionCurrent = _vectorDirection.at(rand() % (_vectorDirection.size()));
            break;
        case GhostState::GoHome:
            target = { 14.f, 11.5f };
            updateDirection = true;
            break;
        case GhostState::Chase:
            switch (GhostNameToInt(this->name)) {
                case 0:
                    target = this->BlinkyUpdate(_pacman);
                    break;
                case 1:
                    target = this->PinkyUpdate(_pacman);
                    break;
                case 2:
                    target = this->InkyUpdate(_pacman, _ghostsContainer);
                    break;
                case 3:
                    target = this->ClydeUpdate(_pacman);
                    break;
            }
            updateDirection = true;
            break;
        case GhostState::Corner:
            target = CORNERS[GhostNameToInt(this->name)];
            updateDirection = true;
            break;
        default:
            break;
	}

	if (updateDirection) {
        this->directionCurrent = this->GetShortestDirection(_vectorDirection, target);
    }

	this->CenterObject();
}

sf::Vector2f Ghosts::BlinkyUpdate(Pacman& _pacman)
{
	return _pacman.GetPosition();
}

sf::Vector2f Ghosts::PinkyUpdate(Pacman& _pacman)
{
	return _pacman.GetPosition() + 
        DIRECTION_ADDITION[static_cast<size_t>(_pacman.GetDirectionCurrent())] * 4.f;
}

sf::Vector2f Ghosts::InkyUpdate(Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer)
{
	sf::Vector2f target = _pacman.GetPosition() + 
        DIRECTION_ADDITION[static_cast<size_t>(_pacman.GetDirectionCurrent())] * 2.f;
	sf::Vector2f offset = 
        _ghostsContainer[GhostNameToInt("Blinky")].GetPosition() - target;
	target = target + offset * -1.f;

	return target;
}

sf::Vector2f Ghosts::ClydeUpdate(Pacman& _pacman)
{
	sf::Vector2f target = _pacman.GetPosition();

	// If clyde is 8 tiles away, target player, else target corner.
	if (Distance(this->position.x, this->position.y, 
        _pacman.GetPosition().x, _pacman.GetPosition().y) < 64
    ) {
        target = { 0.f, 31.f };
    }

	return target;
}

bool Ghosts::FComp(float _a, float _b)
{
	float diff = _a - _b;
	return (diff < 0.01f) && (-diff < 0.01f);
}

bool Ghosts::InTunnel(sf::Vector2f _vector2f)
{
	return _vector2f.x < 0.f || _vector2f.x > 27.f;
}

float Ghosts::Distance(int _x, int _y, int _x1, int _y1)
{
	return ((_x - _x1) * (_x - _x1)) + ((_y - _y1) * (_y - _y1));
}

// The original didn't use  a pathfinding algorithm. Simple distance comparisons are fine.
Direction Ghosts::GetShortestDirection(
    const std::vector<Direction>& _vectorDirection, sf::Vector2f _vector2f
) {
	int minDist = 20000000;
	Direction minDirection = Direction::None;
	if (_vectorDirection.size() == 0) {
        std::cerr << _indent(9) << "# Empty direction for Ghost." << std::endl;
    }

	for (auto dir : _vectorDirection) {
		sf::Vector2f square = 
            this->position + DIRECTION_ADDITION[static_cast<size_t>(dir)];
		float dirDist = Distance(_vector2f.x, _vector2f.y, square.x, square.y);
		if (dirDist <= minDist) {
			minDirection = dir;
			minDist = dirDist;
		}
	}

	return minDirection;
}

void Ghosts::CenterObject()
{
	switch (directionCurrent) {
        case Direction::Up: this->position.x = std::floor(this->position.x) + 0.5f; break;
        case Direction::Down: this->position.x = std::floor(this->position.x) + 0.5f; break;
        case Direction::Left: this->position.y = std::floor(this->position.y) + 0.5f; break;
        case Direction::Right: this->position.y = std::floor(this->position.y) + 0.5f; break;
        case Direction::None: break;
	}
}

bool Ghosts::PassedEntrence()
{
	float prev_X = this->position.x - 
        DIRECTION_ADDITION[static_cast<size_t>(this->directionCurrent)].x * this->speed;

	return FComp(this->position.y, 11.5f) && 
        std::min(this->position.x, prev_X) <= 14.1f && 
        std::max(this->position.x, prev_X) >= 13.9f;
}
