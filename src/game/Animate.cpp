
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "game/Animate.hpp"
#include "Utils.hpp"


Animate::Animate()
{
    std::cout << _indent(3) << "+ Called Animate Constructor." << std::endl;
}

Animate::~Animate()
{
    std::cout << _indent(3) << "- Called Animate Destructor." << std::endl;
}

// PUBLIC - INITIALIZATION
void Animate::Initialize()
{	
	Reset();
}

// PUBLIC - RUNTIME
void Animate::Update(Pacman& _pacman, int _intTimer, int _intEnergizer)
{
	PulseUpdate(_intTimer);

	if (_pacman.GetStopped() && !isDeathAnimation) {
		pacmanFrame = 0;
		assending = true;
	} else {
		pacmanTimer += _intTimer;
	}
	
	if (isDeathAnimation) {
		if (pacmanTimer > 100) {
			pacmanTimer = 0;
			pacmanFrame++;
		}
		if (pacmanFrame > 10) {
			_pacman.SetEnableDraw(false);
		}
	} else if (pacmanTimer > 25 && !_pacman.GetStopped()) {
		if (assending) {
			pacmanFrame += 1;
		} else {
			pacmanFrame -= 1;
		}
		pacmanTimer = 0;
	}
	if ((!isDeathAnimation && pacmanFrame > 2) || pacmanFrame < 0) {
		assending = !assending;
		if (pacmanFrame > 2) {
			pacmanFrame = 2;
		} else {
			pacmanFrame = 0;
		}
	}

	ghostTimer += _intTimer;
	if (ghostTimer > 200) {
		isGhostFrame2 = !isGhostFrame2;
		ghostTimer = 0;
	}

	// Start flashing with 2 seconds to go.
	if (_intEnergizer > 0 && _intEnergizer < 2000) {
		energizerTimer += _intTimer;
		if (energizerTimer > 200) {
			isFrightFlash = !isFrightFlash;
			energizerTimer = 0;
		}
	} else {
		isFrightFlash = false;
	}
}

void Animate::PulseUpdate(int _intTimer)
{
	pulseTimer += _intTimer;
	if (pulseTimer > pulseLimit) {
		isPulse = !isPulse;
		pulseTimer = 0;
	}
}

// PUBLIC - SUPPORT
void Animate::Reset()
{
	pulseTimer = 0;
	isPulse = true;

	pacmanFrame = 0;
	pacmanTimer = 0;
	isDeathAnimation = false;
	
	isFrightFlash = false;
	isGhostFrame2 = false;
	ghostTimer = 0;

	energizerTimer = 0;

	assending = true;
}

void Animate::SetupWindowStateMenu(int _intTimer)
{
	pacmanFrame = 1;
	isDeathAnimation = false;

	isGhostFrame2 = false;

	SetPulseLimit(_intTimer);
}

sf::IntRect Animate::GetPacmanFrame(Direction _direction)
{
	sf::IntRect rect = { 0, 0, 30, 30 };
	rect.left = (2 - pacmanFrame) * 32;

	if (isDeathAnimation) {
		rect.left = 96 + pacmanFrame * 32;
		return rect;
	}
	if (pacmanFrame == 0){
		return rect;
	}
		
	switch (_direction) {
	case Direction::Up:	rect.top = 64; break;
	case Direction::Down: rect.top = 96; break;
	case Direction::Left: rect.top = 32; break;
	case Direction::Right: rect.top = 0; break;
	case Direction::None: break;
	}

	return rect;
}

sf::IntRect Animate::GetGhostFrame(unsigned _unsigned, GhostState _ghostState, Direction _direction)
{
	sf::IntRect frame = { 0, 128, 32, 32 };
	
	int offset = 0;
	if (_ghostState != GhostState::Frightened) {
		switch (_direction) {
		case Direction::Up:	offset = 128; break;
		case Direction::Down: offset = 128 + 64; break;
		case Direction::Left: offset = 64; break;
		case Direction::Right: break;
		case Direction::None: break;
		}
	}
	
	if (_ghostState == GhostState::Frightened) {
		frame.left = 256;
		if (isFrightFlash) {
			frame.left += 64;
		}
	} else if (_ghostState == GhostState::GoHome || _ghostState == GhostState::EnterHome) {
		frame.left = 256 + (offset / 2);
		frame.top = 128 + 32;
	} else {
		frame.top += 32 * _unsigned;
		frame.left = offset;
	}

	if(_ghostState != GhostState::GoHome && _ghostState != GhostState::EnterHome) {
		frame.left += isGhostFrame2 * 32;
	}

	return frame;
}

void Animate::StartPacmanDeath()
{
	isDeathAnimation = true;
	pacmanFrame = 0;
	pacmanTimer = -250;
}
