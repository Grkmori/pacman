
#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game/Animate.hpp"
#include "game/Pacman.hpp"
#include "game/Ghosts.hpp"
#include "game/States.hpp"


class Graphics 
{
private: // Objects, variables and constants.
    // TEXTURES
    static sf::Texture textureFont;
    static sf::Texture textureMap;
    static sf::Texture textureMapWhite;
    static sf::Texture texturePellets;
    static sf::Texture textureSprites;

    // VERTEXES
    static sf::VertexArray vertexText;
    static sf::VertexArray vertexMap;
    static sf::VertexArray vertexPellets;
    std::map<int, int> indiceVertexPellets; // Store map index of pellet location, only delete the one eaten.
    std::vector<int> indiceVertexPowerPellets; // Keep index of power up pellets to be flashed.
    static sf::VertexArray vertexSprites;

    // SPRITES
    sf::Sprite spriteScore;
    sf::Sprite spriteMap;
    sf::Sprite spritePacman;
    std::vector<sf::Sprite> vectorSpriteGhosts;

    bool isPowerOff;

public: // Constructor and functions.
    Graphics();
    ~Graphics();

    // INITIALIZATION
    void Initialize(std::vector<std::string>& _stringVector, std::vector<Ghosts> _ghostsContainer);

    // INTERACTIVE
    void RenderMenuWindow(
        Animate& _animate, Pacman& _pacman, std::vector<Ghosts>& _ghostsVector, int _intTimer
    );
    void DrawMenuWindow(sf::RenderWindow& _window);
    void RenderAndDrawGameWindow(sf::RenderWindow& _window, WindowState& _windowState, 
        int _intScore, int _intHighScore, int _intLives, bool _boolEat, int _intEaten, 
        Animate& _animate, Pacman& _pacman, std::vector<Ghosts>& _ghostsVector
    );

    // SUPPORT
    void Reset();
    void ResetPellets();
    void RemovePellet(int _x, int _y);

private: // Functions.
    // INITIALIZATION
    void LoadTextures();
    void SettingVertexes(std::vector<std::string>& _stringVector);
    void SettingSprites(std::vector<Ghosts> _ghostsContainer);
    
    // SUPPORT
    void LoadVertexMap(sf::VertexArray& _vertexArray, std::vector<std::string> _stringVector);
    void LoadVertexPellets(
        sf::VertexArray& _vertexArray, std::vector<std::string>& _stringVector, 
        std::map<int, int>& _map, std::vector<int>& _vector
    );
    void MakeVertexText(std::string _string, int _x, int _y, sf::Color _color);
    void ClearVertexText();
    void MakeQuad(
        sf::VertexArray& _vertexArray, float _posX, float _posY, int _width, int _height, 
        sf::Color _color = { 255, 255, 255 }, sf::FloatRect _floatRect = { 0, 0, 0, 0 }
    );
    char GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y);
    void FlashPowerPellets();
};
