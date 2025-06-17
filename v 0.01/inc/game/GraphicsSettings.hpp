
#pragma once

#include <SFML/System.hpp>

#include <string>


// MAP
inline const int CELL_SIZE = 8;
inline const int Y_OFFSET = CELL_SIZE * 3;

// TEXTURES
inline const std::string TEXTURE_PATH_FONT = "res/textures/Font.png";
inline const std::string TEXTURE_PATH_MAP = "res/textures/Map.png";
inline const std::string TEXTURE_PATH_MAP_WHITE = "res/textures/MapWhite.png";
inline const std::string TEXTURE_PATH_PELLETS = "res/textures/Pellets.png";
inline const std::string TEXTURE_PATH_SPRITES = "res/textures/Sprites.png";

// SPRITES
inline const float SPRITE_SCALE_DEFAULT = 0.5f;
inline const float SPRITE_ORIGIN_FONT = 16.f;
inline const sf::FloatRect PELLET_RECTANGLE = { 0, 0, 16.f, 16.f };
inline const sf::FloatRect POWER_PELLET_RECTANGLE = { 16.f, 0, 16.f, 16.f };
inline const float SPRITE_ORIGIN_PACMAN = 15.f;
inline const float SPRITE_ORIGIN_GHOST = 16.f;


//inline const int FONT_WIDTH = 14;
