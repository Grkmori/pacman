
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "game/Game.hpp"
#include "Utils.hpp"
#include "FileManager.hpp"
#include "game/GameSettings.hpp"
#include "game/GraphicsSettings.hpp"
#include "game/PacmanSettings.hpp"
#include "game/GhostsSettings.hpp"


Game::Game()
: gameWindow(sf::VideoMode(WINDOW_WIDTH * CELL_SIZE * 2, WINDOW_HEIGHT * CELL_SIZE * 2), GAME_NAME)
{
	std::cout << "+ Called Game Constructor." << std::endl;
}

Game::~Game()
{
	std::cout << "- Called Game Destructor." << std::endl;
}

// PUBLIC - INITIALIZATION
void Game::Initialize(Network& _nn)
{
	// Game Objects and Parameters
	std::cout << "+ Setting up GameWindow." << std::endl;
	gameWindow.setPosition(sf::Vector2i(WINDOW_POS_X, WINDOW_POS_Y));

	gameViewCenter = sf::Vector2f(
		static_cast<float>(WINDOW_WIDTH * CELL_SIZE) / 2.f,
		static_cast<float>(WINDOW_HEIGHT * CELL_SIZE) / 2.f
	);
	gameView.setSize(gameViewCenter.x * 2.f, gameViewCenter.y * 2.f);
	gameView.setCenter(gameViewCenter);
	gameWindow.setView(gameView);
	gameView = gameWindow.getView();

	gameWindow.setFramerateLimit(FPS);

	std::cout << "+ Setting up Graphics." << std::endl;
	LoadMapBoard(mapBoard);

	gameGraphics.Initialize(mapBoard, ghostsContainer);

	std::cout << "+ Setting up Audio." << std::endl;
	gameAudio.Initialize();

	std::cout << "+ Setting up Animate." << std::endl;
	gameAnimate.Initialize();

	std::cout << "+ Setting up Pacman." << std::endl;
	pacman.Initialize();

	std::cout << "+ Setting up Ghosts." << std::endl;
	ghostsContainer.reserve(4);
	for (unsigned i = 0; i < GHOSTS_NAMES.size(); i++) {
		ghostsContainer.emplace_back(GHOSTS_NAMES[i]);
	}

	std::cout << "+ Setting up Game initial configuration." << std::endl;
	windowState = WindowState::Menu;

	fixedAIDelta = AI_DELTA;
	timeElapsed = 0;
	gameStep = false;
	hasStepped = false;

	highScore = LoadHighScore();
	gameScore = 0;

	isFirstLife = true;
	playerLives = 0;

	isGlobalCounter = false;
	globalPelletsCounter = 0;
	pelletsLeft = 0;
    isPelletEaten = false;

    ghostRecentEaten = nullptr;
	isPlayerEatGhost = false;
	ghostsEatenInPowerup = 0;

	std::cout << "+ Setting up NeuralNetwork initial configuration." << std::endl;
	_nn.Initialize(mapBoard);

	std::cout << "+ Setting up WindowState initial configuration (Menu)." << std::endl;
	SetupMenu();
}

// PUBLIC - RUNTIME
void Game::Run(Network& _nn)
{
	std::cout << "+ Starting Game loop..." << std::endl;
	while (gameWindow.isOpen()) {
		sf::Event gameEvent;

		while (gameWindow.pollEvent(gameEvent)) {
			if (gameEvent.type == sf::Event::Closed) {
				std::cout << "- Closing GameWindow." << std::endl;
				SaveHighScore(highScore);
				gameWindow.close();
			}
			if (gameEvent.type == sf::Event::KeyPressed) {
				switch (gameEvent.key.code) {
					case sf::Keyboard::Escape:
						if (windowState == WindowState::Menu) {
							std::cout << "- Pressed 'Escape'. Closing GameWindow." << std::endl;
							SaveHighScore(highScore);
							gameWindow.close();
						}
						break;
					case sf::Keyboard::Enter:
						if (windowState == WindowState::Menu) {
							std::cout << "+ Pressed 'Enter'." << std::endl;
							SetupGameStart();
						}
						break;
					case sf::Keyboard::P: // Toggles between real-time and turn-based mode
						if (windowState == WindowState::MainLoop) {
							_AI_ = !_AI_;
							std::cout << "+ Pressed 'P'. Turn-based mode is " <<
								(_AI_ ? "ON" : "OFF") << "." << std::endl;
						}
						break;
					case sf::Keyboard::Up:
						if (_AI_ && windowState == WindowState::MainLoop) {
							pacman.SetDirectionTry(Direction::Up);
							gameStep = true; 
						} else {
							pacman.SetDirectionTry(Direction::Up);
						}
						break;
					case sf::Keyboard::Down:
						if (_AI_ && windowState == WindowState::MainLoop) {
							pacman.SetDirectionTry(Direction::Down);
							gameStep = true; 
						} else {
							pacman.SetDirectionTry(Direction::Down);
						}
						break;
					case sf::Keyboard::Left:
						if (_AI_ && windowState == WindowState::MainLoop) {
							pacman.SetDirectionTry(Direction::Left);
							gameStep = true; 
						} else {
							pacman.SetDirectionTry(Direction::Left);
						}
						break;
					case sf::Keyboard::Right:
						if (_AI_ && windowState == WindowState::MainLoop) {
							pacman.SetDirectionTry(Direction::Right);
							gameStep = true; 
						} else {
							pacman.SetDirectionTry(Direction::Right);
						}
						break;
					default:
						if (!_AI_) {
							pacman.SetDirectionTry(Direction::None);
						}
				}
			}
		}

		gameWindow.clear(sf::Color::Black);

		if (_AI_) {
			fixedAIDelta -= gameTimeElapsed.asMilliseconds();
			if (fixedAIDelta < 0) {
				switch (_nn.RandomChoice()) {
					case 0:
						pacman.SetDirectionTry(
							RelativeToAbsoluteDirection(pacman.GetDirectionTry(), "None")
						); break;
					case 1:
						pacman.SetDirectionTry(
							RelativeToAbsoluteDirection(pacman.GetDirectionTry(), "Front")
						); break;
					case 2:
						pacman.SetDirectionTry(
							RelativeToAbsoluteDirection(pacman.GetDirectionTry(), "Back")
						); break;
					case 3:
						pacman.SetDirectionTry(
							RelativeToAbsoluteDirection(pacman.GetDirectionTry(), "Left")
						); break;
					case 4:
						pacman.SetDirectionTry(
							RelativeToAbsoluteDirection(pacman.GetDirectionTry(), "Right")
						); break;
					default:
						pacman.SetDirectionTry(Direction::None); break;
				}
				gameStep = true;
				fixedAIDelta = AI_DELTA;
			}
		}

		if (!_AI_ || windowState != WindowState::MainLoop) {
			// This is the original real-time mode logic
			timeElapsed = gameTimeElapsed.asMilliseconds();
			gameTimeElapsed = gameClock.restart();
		} else { // if (_AI_ && windowState == WindowState::MainLoop)
			if (gameStep) {
				timeElapsed = AI_DELTA;
			} else { 
				timeElapsed = 0;
			}
			gameTimeElapsed = gameClock.restart();
		}

		switch (windowState) {
			case WindowState::MainLoop:
				MainLoop();
				if (hasStepped) {
					gameStep = false;
					hasStepped = false;
				}
				break;
			case WindowState::GameStart:
				GameStart();
				gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
					gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
					gameAnimate, pacman, ghostsContainer
				);
				break;
			case WindowState::GameLose:
				GameLose();
				break;
			case WindowState::GameOver:
				pauseTime -= timeElapsed;
				if (pauseTime < 0) {
					SetupMenu();
					windowState = WindowState::Menu;
				}

				gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
					gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
					gameAnimate, pacman, ghostsContainer
				); // TODO: Check if correct.
				break;
			case WindowState::GameWin:
				GameWin();
				break;
			case WindowState::Menu:
				gameGraphics.RenderMenuWindow(
					gameAnimate, pacman, ghostsContainer, timeElapsed
				);
				gameGraphics.DrawMenuWindow(gameWindow);
				break;
		}

		gameWindow.display();
	}
}

// PRIVATE - INTERACTIVE
void Game::SetupMenu()
{
	std::cout << _indent(3) << "+ Setting up windowState Menu." << std::endl;
	gameAnimate.SetupWindowStateMenu(500);

	pacman.SetupWindowStateMenu();
	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		ghostsContainer[i].SetupWindowStateMenu();
	}
}

void Game::SetupGameStart()
{
	std::cout << _indent(3) << "+ Setting up windowState GameStart." << std::endl;
	gameGraphics.ResetPellets();
	ResetBoard();
	ResetEntities();
	ResetTimers();

	pauseTime = DEFAULT_PAUSE_TIME;
	gameScore = 0;
	playerLives = INITIAL_LIVES;

	gameAudio.GameStart();
	windowState = WindowState::GameStart;
}

void Game::GameStart()
{
	//std::cout << _indent(3) << "+ Loading windowState GameStart." << std::endl;
	pauseTime -= timeElapsed;
	if (pauseTime <= 0) {
		windowState = WindowState::MainLoop;
		gameAnimate.SetPulseLimit(150);
	}
}

void Game::MainLoop()
{
	//std::cout << _indent(3) << "+ Running windowState MainLoop." << std::endl;
	if (isPlayerEatGhost) {
		pauseTime -= timeElapsed;
		if (pauseTime < 0) {
			ghostRecentEaten->SetEnableDraw(true);
			pacman.SetEnableDraw(true);
			isPlayerEatGhost = false;
		}
		gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
			gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
			gameAnimate, pacman, ghostsContainer
		);

		return;
	}

	// Pacman doesn't move for one frame if he eats a pellet from the original game.
	if (!isPelletEaten) {
		MovementAndMapCollision();
	} else {
		isPelletEaten = false;
	}
	GhostsCollision();
	PelletsCollision();

	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		ghostsContainer[i].UpdateGhosts(isGlobalCounter, globalPelletsCounter, waveCounter,
			pacman, ghostsContainer, mapBoard, gameStep, hasStepped
		);
	}

	UpdateWave();
	UpdateEnergizerTime();
	CheckHighScore();
	CheckWin();

	gameAudio.UpdateGameSounds(windowState, ghostsContainer,
		timeElapsed, energizerTime, pelletsLeft
	);
	gameAnimate.Update(pacman, timeElapsed, energizerTime);

	gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
		gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
		gameAnimate, pacman, ghostsContainer
	);
}

void Game::GameLose()
{
	pauseTime -= timeElapsed;
	if (pauseTime <= 0) {
		if (playerLives < 0) {
			windowState = WindowState::GameOver;
			pauseTime = 5000;
			for (unsigned i = 0; i < ghostsContainer.size(); i++) {
				ghostsContainer[i].SetEnableDraw(false);
			}
			pacman.SetEnableDraw(false);
		}
		else {
			windowState = WindowState::GameStart;
			pauseTime = 2000;

			ResetEntities();
		}
	}

	gameAudio.UpdateGameSounds(windowState, ghostsContainer,
		timeElapsed, energizerTime, pelletsLeft
	);
	gameAnimate.Update(pacman, timeElapsed, energizerTime);

	gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
		gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
		gameAnimate, pacman, ghostsContainer
	);
}

void Game::GameWin()
{
	pauseTime -= timeElapsed;
	if (pauseTime <= 0) {
		gameGraphics.ResetPellets();
		ResetBoard();
		ResetEntities();
		ResetTimers();
		pauseTime = 2000;
		windowState = WindowState::GameStart;
	}

	gameAnimate.Update(pacman, timeElapsed, energizerTime);

	gameGraphics.RenderAndDrawGameWindow(gameWindow, windowState,
		gameScore, highScore, playerLives, isPlayerEatGhost, ghostsEatenInPowerup,
		gameAnimate, pacman, ghostsContainer
	);
}

// PRIVATE - SUPPORT
void Game::ResetBoard()
{
	if (windowState != WindowState::Menu) {
		mapBoard.clear();
		LoadMapBoard(mapBoard);
	}

	isFirstLife = true;
	isGlobalCounter = false;
	pelletsLeft = PELLET_TOTAL_AMOUNT;
	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		ghostsContainer[i].SetPelletsCounter(0);
	}
}

void Game::ResetEntities()
{
	std::cout << _indent(3) << "+ Reseting Entities." << std::endl;
	if (!isFirstLife) {
		isGlobalCounter = true;
	}
	globalPelletsCounter = 0;

	gameGraphics.Reset();
	gameAnimate.Reset();

	pacman.Reset();
	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		ghostsContainer[i].Reset();
	}
}

void Game::ResetTimers()
{
	energizerTime = 0;
	waveTime = 0;
	waveCounter = 0;
}

void Game::MovementAndMapCollision()
{
	if (pacman.GetDirectionTry() != Direction::None &&
		!pacman.TileCollision(mapBoard) && !pacman.InTunnel()
	) {
		if (pacman.GetDirectionCurrent() !=
			DIRECTION_OPPOSITE[static_cast<size_t>(pacman.GetDirectionCurrent())]
		) {
			pacman.SetCornering(true);
			pacman.SetDirectionCorrection(pacman.GetCorrection());
		}
		pacman.SetDirectionCurrent(pacman.GetDirectionTry());
	}

	if (!pacman.GetStopped()) {
		if (_AI_) {
			if (gameStep) {
				pacman.SetPosition(pacman.GetPosition() +
					DIRECTION_ADDITION[static_cast<size_t>(pacman.GetDirectionCurrent())] *
					SPEED_PACMAN
				);
				hasStepped = true;
			} else {
				pacman.SetPosition(pacman.GetPosition() + NO_MOVEMENT * SPEED_PACMAN);
			}
		} else {
			pacman.SetPosition(pacman.GetPosition() +
				DIRECTION_ADDITION[static_cast<size_t>(pacman.GetDirectionCurrent())] *
				SPEED_PACMAN
			);
		}
	}

	if (pacman.GetCornering()) {
		pacman.Cornering();
	}

	if (pacman.TileCollision(mapBoard)) {
		pacman.ResolveCollision();
		pacman.SetStopped(true);
	} else {
		pacman.SetStopped(false);
	}

	// Tunneling.
	if (pacman.GetPosition().x < -1.f) {
		pacman.SetPosition(pacman.GetPosition() + sf::Vector2f(29.f, 0.f));
	} else if (pacman.GetPosition().x >= 29.f) {
		pacman.SetPosition(pacman.GetPosition() + sf::Vector2f(-29.f, 0.f));
	}
}

void Game::GhostsCollision()
{
	float px = std::floor(pacman.GetPosition().x);
	float py = std::floor(pacman.GetPosition().y);

	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		if (std::floor(ghostsContainer[i].GetPosition().x) == px &&
			std::floor(ghostsContainer[i].GetPosition().y) == py
		) {
			if (ghostsContainer[i].GetGhostState() == GhostState::Frightened) {
				ghostsContainer[i].SetGhostState(GhostState::GoHome);
				ghostRecentEaten = &ghostsContainer[i];
				ghostsEatenInPowerup++;
				gameScore += (pow(2, ghostsEatenInPowerup) * 100);

				isPlayerEatGhost = true;
				pauseTime = 500;

				ghostsContainer[i].SetEnableDraw(false);
				pacman.SetEnableDraw(false);

				gameAudio.PlayEatGhost();
				std::cout << _indent(9) << "+ Ghost: " <<
					ghostsContainer[i].GetName() << " eaten." << std::endl;
				std::cout << _indent(9) << "+ Ghost " << ghostsContainer[i].GetName() << " new state: " <<
                    std::to_string(static_cast<size_t>(ghostsContainer[i].GetGhostState())) << std::endl;
			} else if (ghostsContainer[i].GetGhostState() != GhostState::GoHome) {
				windowState = WindowState::GameLose;
				pauseTime = 2000;
				playerLives -= 1;
				isFirstLife = false;

				gameAnimate.StartPacmanDeath();

				gameAudio.StopMainLoopSounds();
				gameAudio.PlayDeathSound();
			}
		}
	}
}

void Game::PelletsCollision()
{
	char tile = pacman.GetBoardTile(mapBoard, pacman.GetPosition().x, pacman.GetPosition().y);
	bool collided = false;

	if (tile == '.') {
		collided = true;
		gameScore += SCORE_PELLET;
		gameAudio.PlayMunch();
	} else if (tile == 'o') {
		collided = true;
		gameScore += SCORE_POWER_PELLET;
		energizerTime = FRIGHT_TIME * 1000;
		ghostsEatenInPowerup = 0;
		std::cout << _indent(9) << "+ Power pellet eaten." << std::endl;

		for (unsigned i = 0; i < ghostsContainer.size(); i++) {
			ghostsContainer[i].UpdateGhostState(mapBoard, GhostState::Frightened);
		}
	}

	if (collided) {
		gameGraphics.RemovePellet(pacman.GetPosition().x, pacman.GetPosition().y);
		pacman.SetTile(mapBoard, pacman.GetPosition().x, pacman.GetPosition().y, ' ');
		IncrementGhostHouse();
		isPelletEaten = true;
		pelletsLeft--;
	}
}

void Game::IncrementGhostHouse()
{
	Ghosts* tempRecentEaten = ghostRecentEaten;
	ghostRecentEaten = nullptr;

	if (isGlobalCounter) {
		globalPelletsCounter++;
	}

	for (unsigned i = 0; i < ghostsContainer.size(); i++) {
		if (ghostsContainer[i].GetGhostState() == GhostState::HomeBase) {
			ghostRecentEaten = &ghostsContainer[i];
			break;
		}
	}

	if (ghostRecentEaten == nullptr) {
		if (isGlobalCounter) {
			isGlobalCounter = false;
		}
	} else if (!isGlobalCounter) {
		ghostRecentEaten->SetPelletsCounter(ghostRecentEaten->GetPelletsCounter() + 1);
	}

	if (isPlayerEatGhost && tempRecentEaten != nullptr) {
        ghostRecentEaten = tempRecentEaten;
    }
}

void Game::UpdateWave()
{
	if (waveCounter >= 7) {	// Indefinte chase mode.
		return;
	}

	waveTime += timeElapsed;
	if (waveTime / 1000 >= WAVE_TIMES[waveCounter]) { // std 1000
		waveCounter++;
		std::cout << _indent(9) << "+ New wave: " <<
			std::to_string(waveCounter) << "." << std::endl;
		if (energizerTime <= 0) {
			for (unsigned i = 0; i < ghostsContainer.size(); i++) {
				ghostsContainer[i].UpdateGhostState(
					mapBoard, ghostsContainer[i].GetGlobalTarget(waveCounter)
				);
			}
		}
		waveTime = 0;
	}

}

void Game::UpdateEnergizerTime()
{
	energizerTime -= timeElapsed;

	if (energizerTime >= 0) {
		for (unsigned i = 0; i < ghostsContainer.size(); i++) {
			if (ghostsContainer[i].GetGhostState() != GhostState::Frightened) {
				ghostsContainer[i].UpdateGhostState(mapBoard, GhostState::Frightened);
			}
		}
		return;
	}

	if (energizerTime <= 0) {
		for (unsigned i = 0; i < ghostsContainer.size(); i++) {
			if (ghostsContainer[i].GetGhostState() == GhostState::Frightened) {
				ghostsContainer[i].UpdateGhostState(
					mapBoard, ghostsContainer[i].GetGlobalTarget(waveCounter)
				);
			}
		}
	}
}

void Game::CheckHighScore()
{
	if (gameScore > highScore)
		highScore = gameScore;
}

void Game::CheckWin()
{
	if (pelletsLeft <= 0) {
		windowState = WindowState::GameWin;
		for (unsigned i = 0; i < ghostsContainer.size(); i++) {
			ghostsContainer[i].SetEnableDraw(false);
		}
		pacman.SetStopped(true);
		pauseTime = 2000;
		gameAudio.StopMainLoopSounds();
		gameAnimate.SetPulseLimit(200);
	}
}
