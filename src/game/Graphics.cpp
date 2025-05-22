
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <iostream> 

#include "game/Graphics.hpp"
#include "Utils.hpp"
#include "game/GameSettings.hpp"
#include "game/GraphicsSettings.hpp"
#include "game/PacmanSettings.hpp"
#include "game/GhostsSettings.hpp"
#include "game/Direction.hpp"

sf::Texture Graphics::textureFont;
sf::Texture Graphics::textureMap;
sf::Texture Graphics::textureMapWhite;
sf::Texture Graphics::texturePellets;
sf::Texture Graphics::textureSprites;

sf::VertexArray Graphics::vertexText;
sf::VertexArray Graphics::vertexMap;
sf::VertexArray Graphics::vertexPellets;
sf::VertexArray Graphics::vertexSprites;


Graphics::Graphics()
{
	std::cout << _indent(3) << "+ Called Graphics Constructor." << std::endl;
}

Graphics::~Graphics()
{
	std::cout << _indent(3) << "- Called Graphics Destructor." << std::endl;
}

// PUBLIC - INITILIZATION
void Graphics::Initialize(std::vector<std::string>& _stringVector, std::vector<Ghosts> _ghostsContainer)
{
	LoadTextures();
	
	indiceVertexPowerPellets.reserve(4);
	SettingVertexes(_stringVector);

	vectorSpriteGhosts.reserve(4);
	SettingSprites(_ghostsContainer);

	isPowerOff = false;
}

// PUBLIC - INTERACTIVE
void Graphics::RenderMenuWindow(
	Animate& _animate, Pacman& _pacman, std::vector<Ghosts>& _ghostsVector, int _intTimer
) {
	//std::cout << _indent(3) << "+ Rendering Window Menu." << std::endl;
	ClearVertexText();

	MakeVertexText("- PACMAN", 11, 8, TEXT_COLOR_PACMAN);
	MakeVertexText("- BLINKY", 11, 11, TEXT_COLOR_BLINKY);
	MakeVertexText("- PINKY", 11, 14, TEXT_COLOR_PINKY);
	MakeVertexText("- INKY", 11, 17, TEXT_COLOR_INKY);
	MakeVertexText("- CLYDE", 11, 20, TEXT_COLOR_CLYDE);
	
	_animate.PulseUpdate(_intTimer);
	if (_animate.GetIsPulse()) { 
		MakeVertexText("PRESS ENTER", 8, 25, TEXT_COLOR_WHITE);
	}

	spritePacman.setPosition(_pacman.GetPosition().x * CELL_SIZE, 
		_pacman.GetPosition().y * CELL_SIZE + Y_OFFSET
	);
	spritePacman.setTextureRect(_animate.GetPacmanFrame(_pacman.GetDirectionCurrent()));
	
	for (unsigned i = 0; i < vectorSpriteGhosts.size(); i++) {
		vectorSpriteGhosts[i].setPosition(
			_ghostsVector[i].GetPosition().x * CELL_SIZE, 
			_ghostsVector[i].GetPosition().y * CELL_SIZE + Y_OFFSET
		);
		vectorSpriteGhosts[i].setTextureRect(
			_animate.GetGhostFrame(i, 
				_ghostsVector[i].GetGhostState(), _ghostsVector[i].GetDirectionCurrent()
			)
		);
	}
}

void Graphics::DrawMenuWindow(sf::RenderWindow& _window)
{
	//std::cout << _indent(3) << "+ Drawing Window Menu." << std::endl;
	_window.draw(vertexText, &textureFont);
	_window.draw(spritePacman);
	for (unsigned i = 0; i < vectorSpriteGhosts.size(); i++) {
		_window.draw(vectorSpriteGhosts[i]);
	}
}

void Graphics::RenderAndDrawGameWindow(sf::RenderWindow& _window, WindowState& _windowState, 
	int _intScore, int _intHighScore, int _intLives, bool _boolEat, int _intEaten, 
	Animate& _animate, Pacman& _pacman, std::vector<Ghosts>& _ghostsVector
) {
	//std::cout << _indent(3) << "+ Rendering and Drawing Window Game." << std::endl;
	ClearVertexText();

	if (_windowState == WindowState::GameStart) {
		MakeVertexText("Ready!", 11, 20, TEXT_COLOR_YELLOW);
	} else if (_windowState == WindowState::GameOver) {
		MakeVertexText("GAME  OVER", 9, 20, TEXT_COLOR_YELLOW);
	}

	std::string score  = std::to_string(_intScore);
	if (score.size() == 1) {
		score.insert(score.begin(), '0');
	}
	MakeVertexText(score, 7 - score.size(), 1, TEXT_COLOR_WHITE);

	MakeVertexText("High Score", 9, 0, TEXT_COLOR_WHITE);
	score = std::to_string(_intHighScore);
	if (score.size() == 1) {
		score.insert(score.begin(), '0');
	}
	MakeVertexText(score, 17 - score.size(), 1, TEXT_COLOR_WHITE);

	spritePacman.setTextureRect({ 256, 32, 30, 30 });
	for (int i = 0; i < _intLives; i++) {
		spritePacman.setPosition({ 24.f + 16.f * i, 35.f * CELL_SIZE });
		_window.draw(spritePacman);
	}

	if (isPowerOff != _animate.GetIsPulse()) {
		FlashPowerPellets();
		isPowerOff = !isPowerOff;
	}

	static bool whiteTexture = false;
	if (_windowState == WindowState::GameWin) {
		if (_animate.GetIsPulse() && !whiteTexture) {
			spriteMap.setTexture(textureMapWhite);
			whiteTexture = true;
		} else if (!_animate.GetIsPulse() && whiteTexture) {
			spriteMap.setTexture(textureMap);
			whiteTexture = false;
		}
	}

	_window.draw(spriteMap);
	_window.draw(vertexText, &textureFont);
	_window.draw(vertexPellets, &texturePellets);

	for (unsigned i = 0; i < _ghostsVector.size(); i++) {
		if (!_ghostsVector[i].GetEnableDraw()){
			continue;
		}
		vectorSpriteGhosts[i].setPosition(
			_ghostsVector[i].GetPosition().x * CELL_SIZE, 
			_ghostsVector[i].GetPosition().y * CELL_SIZE + Y_OFFSET
		);
		vectorSpriteGhosts[i].setTextureRect(
			_animate.GetGhostFrame(i, _ghostsVector[i].GetGhostState(), 
				_ghostsVector[i].GetDirectionCurrent()
			)
		);
		_window.draw(vectorSpriteGhosts[i]);
	}

	if (_pacman.GetEnableDraw()) {
		spritePacman.setPosition(_pacman.GetPosition().x * CELL_SIZE, 
			_pacman.GetPosition().y * CELL_SIZE + Y_OFFSET
		);
		spritePacman.setTextureRect(_animate.GetPacmanFrame(_pacman.GetDirectionCurrent()));
		_window.draw(spritePacman);
	}

	if (_boolEat) {
		spriteScore.setPosition(
			_pacman.GetPosition().x * CELL_SIZE, _pacman.GetPosition().y * CELL_SIZE + Y_OFFSET);
		spriteScore.setTextureRect({ (_intEaten - 1) * 32, 256, 32, 32 });
		_window.draw(spriteScore);
	}
}

// PUBLIC - SUPPORT
void Graphics::Reset()
{
	isPowerOff = true;
	FlashPowerPellets();
}

void Graphics::ResetPellets()
{
	spriteMap.setTexture(textureMap);
	
	for (unsigned i = 0; i < vertexPellets.getVertexCount(); i++) {
		vertexPellets[i].color = TEXT_COLOR_WHITE;
	}
}

void Graphics::RemovePellet(int _x, int _y)
{
	int va_idx = indiceVertexPellets.at(_y * 28 + _x);
	vertexPellets[va_idx].color = { 0, 0, 0, 0 };
	vertexPellets[va_idx].color = { 0, 0, 0, 0 };
	vertexPellets[va_idx].color = { 0, 0, 0, 0 };
	vertexPellets[va_idx].color = { 0, 0, 0, 0 };
}

// PRIVATE - INITIALIZATION
void Graphics::LoadTextures()
{
	std::cout << _indent(3) << "+ Loading Textures." << std::endl;
	try {
        if (!textureFont.loadFromFile(TEXTURE_PATH_FONT)) {
            throw std::runtime_error("Failed to load Font.png");
            std::cerr << "# textureFont.png has not been loaded."<< std::endl;
        }
		if (!textureMap.loadFromFile(TEXTURE_PATH_MAP)) {
            throw std::runtime_error("Failed to load Map.png");
            std::cerr << "# textureMap.png has not been loaded."<< std::endl;
        }
		if (!textureMapWhite.loadFromFile(TEXTURE_PATH_MAP_WHITE)) {
            throw std::runtime_error("Failed to load MapWhite.png");
            std::cerr << "# textureMapWhite has not been loaded."<< std::endl;
        }
		if (!texturePellets.loadFromFile(TEXTURE_PATH_PELLETS)) {
            throw std::runtime_error("Failed to load Pellets.png");
            std::cerr << "# texturePellets.png has not been loaded."<< std::endl;
        }
		if (!textureSprites.loadFromFile(TEXTURE_PATH_SPRITES)) {
            throw std::runtime_error("Failed to load Sprites.png");
            std::cerr << "# textureSprites.png has not been loaded."<< std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "# Error: " << e.what() << std::endl;
    }
}

void Graphics::SettingVertexes(std::vector<std::string>& _stringVector)
{
	std::cout << _indent(3) << "+ Setting Vertexes." << std::endl;
	vertexText.setPrimitiveType(sf::Quads);
	
	vertexMap.setPrimitiveType(sf::Quads);
	LoadVertexMap(vertexMap, _stringVector);
	
	vertexPellets.setPrimitiveType(sf::Quads);
	LoadVertexPellets(vertexPellets, _stringVector, indiceVertexPellets, indiceVertexPowerPellets);
	
	vertexSprites.setPrimitiveType(sf::Quads);
}

void Graphics::SettingSprites(std::vector<Ghosts> _ghostsContainer)
{
	std::cout << _indent(3) << "+ Setting Sprites." << std::endl;
	spriteScore.setTexture(textureSprites);
	spriteScore.setScale({ SPRITE_SCALE_DEFAULT, SPRITE_SCALE_DEFAULT });
	spriteScore.setOrigin({ SPRITE_ORIGIN_FONT, SPRITE_ORIGIN_FONT });
	
	spriteMap.setTexture(textureMap);
	spriteMap.setScale({ SPRITE_SCALE_DEFAULT, SPRITE_SCALE_DEFAULT });
	
	spritePacman.setTexture(textureSprites);
	spritePacman.setScale({ SPRITE_SCALE_DEFAULT, SPRITE_SCALE_DEFAULT });
	spritePacman.setOrigin({ SPRITE_ORIGIN_PACMAN, SPRITE_ORIGIN_PACMAN });
	

	for (unsigned i = 0; i < GHOSTS_NAMES.size(); i++) {
		sf::Sprite sprite;
		sprite.setTexture(textureSprites);
		sprite.setScale({ SPRITE_SCALE_DEFAULT, SPRITE_SCALE_DEFAULT });
		sprite.setOrigin({ SPRITE_ORIGIN_GHOST, SPRITE_ORIGIN_GHOST });
		vectorSpriteGhosts.emplace_back(sprite);
	}
}

// PRIVATE - SUPPORT
void Graphics::LoadVertexMap(sf::VertexArray& _vertexArray, std::vector<std::string> _stringVector) 
{
	for (unsigned y = 0; y < _stringVector.size(); y++) {
		for (unsigned x = 0; x < _stringVector.at(y).size(); x++) {
			if(_stringVector.at(y).at(x)== '|')
				MakeQuad(
					_vertexArray, x * CELL_SIZE, y * CELL_SIZE + Y_OFFSET, CELL_SIZE, CELL_SIZE, 
					TEXT_COLOR_GRAY
				);
		}
	}
}

void Graphics::LoadVertexPellets( 
	sf::VertexArray& _vertexArray, std::vector<std::string>& _stringVector, 
	std::map<int, int>& _map, std::vector<int>& _vector
) { 
	_vertexArray.clear();
	int pelletIndex = 0;
	int powerPelletIndex = 0;

	for (unsigned y = 0; y < _stringVector.size(); y++) {
		for (unsigned x = 0; x < _stringVector.at(y).size(); x++) {
			char temp = GetBoardTile(_stringVector, x, y);
			if (temp == '.') {
				MakeQuad(
					_vertexArray, x * CELL_SIZE, y * CELL_SIZE + Y_OFFSET, CELL_SIZE, CELL_SIZE, 
					TEXT_COLOR_WHITE, PELLET_RECTANGLE
				);
				_map.insert({ (y * WINDOW_WIDTH) + x, pelletIndex });
				pelletIndex += 4;
			} else if (temp == 'o') {
				MakeQuad(
					_vertexArray, x * CELL_SIZE, y * CELL_SIZE + Y_OFFSET, CELL_SIZE, CELL_SIZE, 
					TEXT_COLOR_WHITE, POWER_PELLET_RECTANGLE
				);
				_map.insert({ (y * WINDOW_WIDTH) + x, pelletIndex });
				_vector.emplace_back(pelletIndex);
				powerPelletIndex++;
				pelletIndex += 4;
			}
		}
	}
}

void Graphics::MakeVertexText(std::string _string, int _x, int _y, sf::Color _color) 
{
	for (unsigned i = 0; i < _string.size(); i++) {
		sf::FloatRect fontLetter = { 0, 0, 16, 16 };
		char letter = toupper(_string.at(i));
		fontLetter.left = (letter - ' ') * 16 + 1;
		MakeQuad(vertexText, _x * CELL_SIZE + CELL_SIZE * i, 
			_y * CELL_SIZE, CELL_SIZE, CELL_SIZE, _color, fontLetter
		);
	}
}

void Graphics::ClearVertexText()
{
	vertexText.clear();
}

void Graphics::MakeQuad( 
    sf::VertexArray& _vertexArray, float _posX, float _posY, int _width, int _height, 
    sf::Color _color, sf::FloatRect _floatRect
) {
	sf::Vertex vert;
	vert.color = _color;
	vert.position = { _posX, _posY };
	vert.texCoords = { _floatRect.left, _floatRect.top };
	_vertexArray.append(vert);
	vert.position = { _posX + _width, _posY };
	vert.texCoords = { _floatRect.left + _floatRect.width, _floatRect.top };
	_vertexArray.append(vert);
	vert.position = { _posX + _width, _posY + _height };
	vert.texCoords = { _floatRect.left + _floatRect.width, _floatRect.top + _floatRect.height };
	_vertexArray.append(vert);
	vert.position = { _posX, _posY + _height };
	vert.texCoords = { _floatRect.left, _floatRect.top + _floatRect.height };
	_vertexArray.append(vert);
}

char Graphics::GetBoardTile(std::vector<std::string>& _stringVector, int _x, int _y)
{
	if (_x < 0 || _x >= static_cast<int>(_stringVector.at(_y).size())) { 
		return '/'; // For tunnel, "/" not used for anything.
	}

	return _stringVector.at(_y).at(_x);
}

void Graphics::FlashPowerPellets()
{
	sf::Uint8 newAlpha; 
	if (isPowerOff) {
		newAlpha = 255;
	} else {
		newAlpha = 1;
	}

	for (unsigned i = 0; i < 4; i++) {
		int index = indiceVertexPowerPellets[i];
		sf::Vertex* vert = &vertexPellets[index];

		if (vert->color.a == 0) { 
			continue;
		}
		vert[0].color.a = newAlpha;
		vert[1].color.a = newAlpha;
		vert[2].color.a = newAlpha;
		vert[3].color.a = newAlpha;
	}
}
