
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "Utils.hpp"
#include "game/GameSettings.hpp"


int LoadHighScore()
{
	std::string line;
	std::ifstream infile;
	std::stringstream ss;
	int hs = 0;

	try {
		infile.open(HIGH_SCORE_PATH);
		if (!infile.is_open()) {
			throw std::runtime_error("Failed to open HighScore.txt");
			std::cerr << "# HighScore.txt has not been oppened."<< std::endl;
		}
	} catch (const std::exception& e) {
		std::cerr << "# Error: " << e.what() << std::endl;
	}

	getline(infile, line);
	ss << line;
	ss >> hs;

	std::cout << _indent(3) << "+ highScore has been loaded." << std::endl;
	infile.close();
	return hs;
}

void SaveHighScore(int _hs)
{
	std::ofstream outfile;

	try {
		outfile.open(HIGH_SCORE_PATH);
		if (!outfile.is_open()) {
			throw std::runtime_error("Failed to open HighScore.txt");
			std::cerr << "# HighScore.txt has not been oppened."<< std::endl;
		}
	} catch (const std::exception& e) {
		std::cerr << "# Error: " << e.what() << std::endl;
	}

	outfile << _hs;

	std::cout << _indent(3) << "+ highScore has been saved." << std::endl;
	outfile.close();
}

void LoadMapBoard(std::vector<std::string>& _stringVector)
{
	std::string line;
	std::ifstream infile;
	
	try {
		infile.open(MAP_BOARD_PATH);
		if (!infile.is_open()) {
			throw std::runtime_error("Failed to open Map.txt");
			std::cerr << "# Map.txt has not been oppened."<< std::endl;
		}
	} catch (const std::exception& e) {
		std::cerr << "# Error: " << e.what() << std::endl;
	}

	while (getline(infile, line)) {
		_stringVector.push_back(line);
		std::cout << line << std::endl;
	}

	size_t height = _stringVector.size();
	size_t width = 0;
	for (const auto& line : _stringVector) {
		if (line.size() > width)
			width = line.size();
	}
	std::cout << _indent(3) << "+ mapBoard has been loaded. Size (HxW): " <<
		std::to_string(height) << "." << std::to_string(width) << "." << std::endl;
	infile.close();
}