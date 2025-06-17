
#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <array>


// GENERAL
inline bool _AI_ = true;

inline const std::string GAME_NAME = "PACMAN";
inline const int FPS = 60;

inline const int DEFAULT_PAUSE_TIME = 4000;
inline const float AI_DELTA = static_cast<int>(1.f / static_cast<float>(FPS) * 1000);
inline const sf::Vector2f NO_MOVEMENT = { 0.f, 0.f };
inline const std::array<int, 8> WAVE_TIMES = { 7, 20, 7, 20, 5, 20, 5, -1 };
inline const int FRIGHT_TIME = 6;

// WINDOW
inline const int WINDOW_WIDTH = 28;
inline const int WINDOW_HEIGHT = 36;
inline const float WINDOW_RATIO = static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT);
inline const int WINDOW_POS_X = 100;
inline const int WINDOW_POS_Y = 0;

// ENTITIES
inline const int PELLET_TOTAL_AMOUNT = 244;
inline const int SCORE_PELLET = 10;
inline const int SCORE_POWER_PELLET = 50;

inline const int INITIAL_LIVES = 3;
inline const float SPEED_FULL = 0.1f; // 0.2f

// FILE MANAGEMENT
inline const std::string HIGH_SCORE_PATH = "res/HighScore.txt";
inline const std::string MAP_BOARD_PATH = "res/Map.txt";

// TEXT COLOR
inline const sf::Color TEXT_COLOR_BLACK = { 0, 0, 0 };
inline const sf::Color TEXT_COLOR_WHITE = { 255, 255, 255 };
inline const sf::Color TEXT_COLOR_GRAY = { 150, 150, 150 };
inline const sf::Color TEXT_COLOR_YELLOW = { 255, 255, 0 };

inline const sf::Color TEXT_COLOR_PACMAN = { 255, 255, 0 };
inline const sf::Color TEXT_COLOR_BLINKY = { 255, 0, 0 };
inline const sf::Color TEXT_COLOR_PINKY = { 252, 181, 255 };
inline const sf::Color TEXT_COLOR_INKY = { 0, 255, 255 };
inline const sf::Color TEXT_COLOR_CLYDE = { 248, 187, 85 };


//inline const int BSIZE = (28 + 1) * 31;
