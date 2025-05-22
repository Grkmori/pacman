
#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "game/Pacman.hpp"
#include "game/States.hpp"
#include "game/Direction.hpp"


class Animate
{
private: // Objects, variables and constants.
	// PULSE
	int pulseTimer;
	int pulseLimit;
	bool isPulse;

	int pacmanFrame;
	int pacmanTimer;
	bool isDeathAnimation;

	int ghostTimer;
	bool isGhostFrame2;
	bool isFrightFlash;

	int energizerTimer;

	bool assending;

public: // Constructor and functions.
    Animate();
    ~Animate();

	// INITIALIZATION
	void Initialize();

	// RUNTIME
	void Update(Pacman& _pacman, int _intTimer, int _intEnergizer);
	void PulseUpdate(int _intTimer);

	// SUPPORT
	void Reset();
	void SetupWindowStateMenu(int _intTimer);
	sf::IntRect GetPacmanFrame(Direction _direction);
	sf::IntRect GetGhostFrame(unsigned _unsigned, GhostState _ghostState, Direction _direction);
	void StartPacmanDeath();

	// GETTER AND SETTERS
	void SetPulseLimit(int _intTimer) { pulseLimit = _intTimer; }
	bool GetIsPulse() const { return isPulse; }
};
