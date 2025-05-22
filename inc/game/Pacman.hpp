
#pragma once

#include <SFML/System.hpp>

#include "game/Direction.hpp"


class Pacman
{
private: // Objects, variables and constants.
    // GENERAL PARAMETERS
	sf::Vector2f position;
	Direction directionCurrent;
	Direction directionTry;
    Direction directionCorrection;

	bool stopped;
	bool cornering;
    
    bool enableDraw;

public: // Constructor and functions.
    Pacman();
    ~Pacman();

    // INITIALIZATION
    void Initialize();

    // INTERACTIVE
    void SetupWindowStateMenu();

    // SUPPORT
    void Reset();
    bool TileCollision(std::vector<std::string>& _stringVector);
    bool InTunnel();
    Direction GetCorrection();
    void Cornering();
    void ResolveCollision();
    char GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y);
    void SetTile(std::vector<std::string>& _stringVector, int _x, int _y, char _charNew);

    // GETTER AND SETTERS
    sf::Vector2f GetPosition() const { return position; }
    void SetPosition(sf::Vector2f _vector2f) { position = _vector2f; }
    Direction GetDirectionCurrent() const { return directionCurrent; }
    void SetDirectionCurrent(Direction _direction) { directionCurrent = _direction; }
    Direction GetDirectionTry() const { return directionTry; }
    void SetDirectionTry(Direction _direction) { directionTry = _direction; }
    Direction GetDirectionCorrection() const { return directionCorrection; }
    void SetDirectionCorrection(Direction _direction) { directionCorrection = _direction; }

    bool GetStopped() const { return stopped; }
    void SetStopped(bool _bool) { stopped = _bool; }
    bool GetCornering() const { return cornering; }
    void SetCornering(bool _bool) { cornering = _bool; }

    bool GetEnableDraw() const { return enableDraw; }
    void SetEnableDraw(bool _bool) { enableDraw = _bool; }

private:
    void CenterObject();
};
