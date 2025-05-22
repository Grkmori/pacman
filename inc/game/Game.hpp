
#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game/Graphics.hpp"
#include "game/Audio.hpp"
#include "game/Animate.hpp"
#include "game/Pacman.hpp"
#include "game/Ghosts.hpp"
#include "game/States.hpp"


class Game 
{
private: // Objects, variables and constants.
    // GAME OBJECTS AND PARAMETERS
    sf::RenderWindow gameWindow;
    sf::View gameView;
    sf::Vector2f gameViewCenter;

    WindowState windowState;

    Graphics gameGraphics;
    Audio gameAudio;
    Animate gameAnimate;

    Pacman pacman;
    std::vector<Ghosts> ghostsContainer;

    std::vector<std::string> mapBoard;

    // CLOCK AND TIMERS (in milliseconds)
    sf::Clock gameClock;
	sf::Time gameTimeElapsed;

    int pauseTime;
	int energizerTime;
	int waveTime;
    int waveCounter;
    
    // PLAYER VARIABLES
    int highScore;
    int gameScore;

	bool isFirstLife;
    int playerLives;

	bool isGlobalCounter;
	int globalPelletsCounter;
    int pelletsLeft;
    bool isPelletEaten;

    Ghosts* ghostRecentEaten;
    bool isPlayerEatGhost;
	int ghostsEatenInPowerup;

	//int current_level = 1; // TODO: Add game levels.

public: // Constructor and functions.
    Game();
    ~Game();

    // INITIALIZATION
    void Initialize();

    // RUNTIME
    void Run();

private: // Functions.
    // INTERACTIVE
    void SetupMenu();
    void SetupGameStart();
    void GameStart();
    void MainLoop();
    void GameLose();
    void GameWin();

    // SUPPORT
    void ResetBoard();
    void ResetEntities();
    void ResetTimers();
    void MovementAndMapCollision();
    void GhostsCollision();
    void PelletsCollision();
    void IncrementGhostHouse();
    void UpdateWave();
    void UpdateEnergizerTime();
    void CheckHighScore();
    void CheckWin();

    // GETTERS AND SETTERS
    void SetWindowState(WindowState _windowState) { windowState = _windowState; }
};
