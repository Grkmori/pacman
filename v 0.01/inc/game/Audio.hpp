
#pragma once

#include <SFML/Audio.hpp>

#include "States.hpp"
#include "game/Ghosts.hpp"


class Audio 
{
private: // Objects, variables and constants.
    // SOUDBUFFERS
	static sf::SoundBuffer bufferStartGame;
    static sf::SoundBuffer bufferMunch1;
	static sf::SoundBuffer bufferMunch2;
	static sf::SoundBuffer bufferEatGhost;
	static sf::SoundBuffer bufferPowerPellet;
	static sf::SoundBuffer bufferRetreating;
	static sf::SoundBuffer bufferSiren;
	static sf::SoundBuffer bufferDeath1;
	static sf::SoundBuffer bufferDeath2;

    // SOUNDS
	sf::Sound soundBackground;
	sf::Sound soundMunch;
	sf::Sound soundEatGhost;
    sf::Sound soundDeath1;
	sf::Sound soundDeath2;

	// AUDIO VARIABLES
	AudioBackgroundState audioBackground;

	bool isFirstMunch;
	bool isPlayingDeath;
	int deathTimer;

public: // Constructor and functions.
    Audio();
    ~Audio();

    // INITIALIZATION
	void Initialize();

	// SUPPORT
	void GameStart();
	void PlayMunch();
	void PlayEatGhost();
	void StopMainLoopSounds();
	void PlayDeathSound();
	void UpdateGameSounds(WindowState _windowState, std::vector<Ghosts>& _ghostsContainer, 
		int _intTimer, int _intEnergizer, int _intPelletsLeft
	);

private: // Functions.
	// INITIALIZATION
    void LoadSoundBuffers();
	void SettingSounds();
	bool GhostsRetreating(std::vector<Ghosts>& _ghostsContainer);
};