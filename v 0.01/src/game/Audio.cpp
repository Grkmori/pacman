
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <exception>

#include "game/Audio.hpp"
#include "Utils.hpp"
#include "game/AudioSettings.hpp"

sf::SoundBuffer Audio::bufferStartGame;
sf::SoundBuffer Audio::bufferMunch1;
sf::SoundBuffer Audio::bufferMunch2;
sf::SoundBuffer Audio::bufferEatGhost;
sf::SoundBuffer Audio::bufferPowerPellet;
sf::SoundBuffer Audio::bufferRetreating;
sf::SoundBuffer Audio::bufferSiren;
sf::SoundBuffer Audio::bufferDeath1;
sf::SoundBuffer Audio::bufferDeath2;


Audio::Audio()
{
    std::cout << _indent(3) << "+ Called Audio Constructor." << std::endl;
}

Audio::~Audio()
{
    std::cout << _indent(3) << "- Called Audio Destructor." << std::endl;
}

// PUBLIC - INITIALIZATION
void Audio::Initialize()
{
    LoadSoundBuffers();
    SettingSounds();

    std::cout << _indent(3) << "+ Setting Audio initial configuration." << std::endl;
	audioBackground = AudioBackgroundState::NoSound;

    isFirstMunch = true;
	isPlayingDeath = false;
	deathTimer = TOTAL_DEATH_TIME;
}

// PUBLIC - SUPPORT
void Audio::GameStart()
{
	soundBackground.stop();
	soundBackground.setPitch(1);
	soundBackground.setBuffer(bufferStartGame);
	soundBackground.play();
}

void Audio::PlayMunch()
{
	if (isFirstMunch) {
        soundMunch.setBuffer(bufferMunch1);
    } else {
        soundMunch.setBuffer(bufferMunch2);
    } 

	isFirstMunch = !isFirstMunch;

	soundMunch.play();
}

void Audio::PlayEatGhost()
{
	soundEatGhost.play();
}

void Audio::StopMainLoopSounds()
{
	soundBackground.stop();
	audioBackground = AudioBackgroundState::NoSound;
}

void Audio::PlayDeathSound()
{
	isPlayingDeath = true;
}

void Audio::UpdateGameSounds(WindowState _windowState, std::vector<Ghosts>& _ghostsContainer, 
    int _intTimer, int _intEnergizer, int _intPelletsLeft
) {
	if (isPlayingDeath) {
		if (deathTimer <= TOTAL_DEATH_TIME - 250 && 
            soundDeath1.getStatus() != sf::SoundSource::Status::Playing
        ) {
			soundDeath1.play();
		}

		deathTimer -= _intTimer;

		if (deathTimer <= 0) {
			soundDeath2.play();
			deathTimer = TOTAL_DEATH_TIME;
			isPlayingDeath = false;
		}
	}
	
	if (_windowState != WindowState::MainLoop) {
        return;
    }
		
	bool updateSound = false;
	if (GhostsRetreating(_ghostsContainer)) {
		if (audioBackground != AudioBackgroundState::Retreat) {
			updateSound = true;
            soundBackground.stop();
			audioBackground = AudioBackgroundState::Retreat;
		}
	} else if (_intEnergizer > 0) {
		if (audioBackground != AudioBackgroundState::PowerPellet) {
			updateSound = true;
            soundBackground.stop();
			audioBackground = AudioBackgroundState::PowerPellet;
		}
	} else if (_windowState == WindowState::MainLoop){
		if (audioBackground != AudioBackgroundState::Siren) {
			updateSound = true;
            soundBackground.stop();
			audioBackground = AudioBackgroundState::Siren;
		}
	}

	if (updateSound) {
		switch (audioBackground) {
            case AudioBackgroundState::Siren:
                soundBackground.setPitch(PITCHES[int((-4.f / 244.f) * _intPelletsLeft + 4)]);
                soundBackground.setBuffer(bufferSiren);
                std::cout << _indent(9) << "+ Background sound updated: Siren." << std::endl;
                break;
            case AudioBackgroundState::Retreat:
                soundBackground.setPitch(1);
                soundBackground.setBuffer(bufferRetreating);
                std::cout << _indent(9) << "+ Background sound updated: Retreat." << std::endl;
                break;
            case AudioBackgroundState::PowerPellet:
                soundBackground.setPitch(1);
                soundBackground.setBuffer(bufferPowerPellet);
                std::cout << _indent(9) << "+ Background sound updated: PowerPellet." << std::endl;
                break;
            case AudioBackgroundState::NoSound: break;
		}

		soundBackground.play();
	}
} 

// PRIVATE - INITIALIZATION
void Audio::LoadSoundBuffers()
{
    std::cout << _indent(3) << "+ Loading SoundBuffers." << std::endl;
    try {
        if (!bufferStartGame.loadFromFile(AUDIO_PATH_START_GAME)) {
            throw std::runtime_error("Failed to load StartGame.wav");
            std::cerr << "# bufferStartGame.wav has not been loaded."<< std::endl;
        }
        if (!bufferMunch1.loadFromFile(AUDIO_PATH_MUNCH1)) {
            throw std::runtime_error("Failed to load Munch1.wav");
            std::cerr << "# Munch1.wav has not been loaded."<< std::endl;
        }
        if (!bufferMunch2.loadFromFile(AUDIO_PATH_MUNCH2)) {
            throw std::runtime_error("Failed to load Munch2.wav");
            std::cerr << "# Munch2.wav has not been loaded."<< std::endl;
        }
        if (!bufferEatGhost.loadFromFile(AUDIO_PATH_EAT_GHOST)) {
            throw std::runtime_error("Failed to load EatGhost.wav");
            std::cerr << "# EatGhost.wav has not been loaded."<< std::endl;
        }
        if (!bufferPowerPellet.loadFromFile(AUDIO_PATH_POWER_PELLET)) {
            throw std::runtime_error("Failed to load PowerPellet.wav");
            std::cerr << "# PowerPellet.wav has not been loaded."<< std::endl;
        }
        if (!bufferRetreating.loadFromFile(AUDIO_PATH_RETREATING)) {
            throw std::runtime_error("Failed to load Retreating.wav");
            std::cerr << "# Retreating.wav has not been loaded."<< std::endl;
        }
        if (!bufferSiren.loadFromFile(AUDIO_PATH_SIREN)) {
            throw std::runtime_error("Failed to load Siren.wav");
            std::cerr << "# Siren3.wav has not been loaded."<< std::endl;
        }
        if (!bufferDeath1.loadFromFile(AUDIO_PATH_DEATH1)) {
            throw std::runtime_error("Failed to load Death1.wav");
            std::cerr << "# Death1.wav has not been loaded."<< std::endl;
        }
        if (!bufferDeath2.loadFromFile(AUDIO_PATH_DEATH2)) {
            throw std::runtime_error("Failed to load Death2.wav");
            std::cerr << "# Death2.wav has not been loaded."<< std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "# Error: " << e.what() << std::endl;
    }
}

void Audio::SettingSounds()
{
    std::cout << _indent(3) << "+ Setting Sounds." << std::endl;
    soundBackground.setLoop(true);

    soundEatGhost.setBuffer(bufferEatGhost);

	soundDeath1.setBuffer(bufferDeath1);
	soundDeath2.setBuffer(bufferDeath2);
}

bool Audio::GhostsRetreating(std::vector<Ghosts>& _ghostsContainer)
{
	for (unsigned i = 0; i < _ghostsContainer.size(); i++) {
		if (_ghostsContainer[i].GetGhostState() == GhostState::GoHome) {
            return true;
        }
	}

	return false;
}
