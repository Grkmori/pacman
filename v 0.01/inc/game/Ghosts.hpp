
#pragma once

#include <SFML/System.hpp>

#include <string>
#include <vector>

#include "game/GhostsSettings.hpp"
#include "Direction.hpp"
#include "States.hpp"
#include "game/Pacman.hpp"


class Ghosts
{
private: // Objects, variables and constants.
    // GENERAL PARAMETERS
    std::string name;
    GhostState state;
	
    sf::Vector2f position;
    Direction directionCurrent;
    bool updateDirection;
    float speed;
	
    bool isInHouse;
	int pelletsCounter;

    bool enableDraw;

public: // Constructor and functions.
    Ghosts(std::string _name);
    ~Ghosts();


    // INITIALIZATION
    void Initialize();

    // INTERACTIVE
    void SetupWindowStateMenu();

    // SUPPORT
    void Reset();
    void UpdateGhostState(std::vector<std::string>& _stringVector, GhostState _state);
    void UpdateGhosts(bool _boolCounter, int _intPelletCounter, int _intWaveCounter,
        Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer,
        std::vector<std::string>& _stringVector, bool _boolStep, bool _boolStepped
    );
    GhostState GetGlobalTarget(int _intWaveCounter);

    // GETTER AND SETTERS
    std::string GetName() const { return name; }
    void SetName(std::string _name) { name = _name; }
    GhostState GetGhostState() const { return state; }
    void SetGhostState(GhostState _state) { state = _state; }
    
    sf::Vector2f GetPosition() const { return position; }
    void SetPosition(sf::Vector2f _vector2f) { position = _vector2f; }
    Direction GetDirectionCurrent() const { return directionCurrent; }
    void SetDirectionCurrent(Direction _direction) { directionCurrent = _direction; }

    int GetIsInHouse() const { return isInHouse; }
    int GetPelletsCounter() const { return pelletsCounter; }
    void SetPelletsCounter(int _int) { pelletsCounter = _int; }

    bool GetEnableDraw() const { return enableDraw; }
    void SetEnableDraw(bool _bool) { enableDraw = _bool; }

private:
    // SUPPORT
    Direction GetOppositeTile(std::vector<std::string>& _stringVector);
    std::vector<Direction> GetAvailableSquares(std::vector<std::string>& _stringVector, 
        sf::Vector2f _vector2f, Direction _direction, bool _boolHomeTile = false
    );
    bool TileCollision(std::vector<std::string>& _stringVector, 
        sf::Vector2f _vector2f, bool  _boolHomeTile = false
    );
    char GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y);
    void HouseUpdate(bool _boolCounter, int _intPelletCounter, int _intWaveCounter);
    bool InMiddleTile(sf::Vector2f _vector2fPosition, sf::Vector2f _vector2fPrevious, 
        Direction _diretion
    );
    void UpdateDirection(const std::vector<Direction>& _vectorDirection,
        Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer
    );
    sf::Vector2f BlinkyUpdate(Pacman& _pacman);
    sf::Vector2f PinkyUpdate(Pacman& _pacman);
    sf::Vector2f InkyUpdate(Pacman& _pacman, std::vector<Ghosts>& _ghostsContainer);
    sf::Vector2f ClydeUpdate(Pacman& _pacman);
    bool FComp(float _a, float _b);
    bool InTunnel(sf::Vector2f _vector2f);
    float Distance(int _x, int _y, int _x1, int _y1);
    Direction GetShortestDirection(
        const std::vector<Direction>& _vectorDirection, sf::Vector2f _vector2f
    );
    void CenterObject();
    bool PassedEntrence();
};
