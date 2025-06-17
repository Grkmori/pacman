
#pragma once

#include <string>
#include <sstream>


enum class WindowState
{
	Menu,
	GameStart,
	MainLoop,
	GameWin,
	GameLose,
	GameOver,
};

enum class AudioBackgroundState
{
	NoSound,
	Siren,
	PowerPellet,
	Retreat,
};

enum class GhostState
{
	Chase, // #0.
	Corner, // #1.
	Frightened, // #2.
	GoHome,	// #3. Inital state to pathfind to the square above the door.
	HomeBase, // #4. Indefinite state going up and down in home.
	LeaveHome, // #5.
	EnterHome, // #6.
};

// SUPPORT
inline std::string WindowStateToString(WindowState _windowState)
{
	std::stringstream ss;
	switch (_windowState) {
		case WindowState::Menu: ss << "Menu"; break;
		case WindowState::GameStart: ss << "GameStart"; break;
		case WindowState::MainLoop: ss << "MainLoop"; break;
		case WindowState::GameWin: ss << "GameWin"; break;
		case WindowState::GameLose: ss << "GameLose"; break;
		case WindowState::GameOver: ss << "GameOver"; break;
		default: ss << "Unknown State"; break;
	}

	return ss.str();
}

inline std::string AudioBackgroundStateToString(AudioBackgroundState _audioState)
{
	std::stringstream ss;
	switch (_audioState) {
		case AudioBackgroundState::NoSound: ss << "NoSound"; break;
		case AudioBackgroundState::Siren: ss << "Siren"; break;
		case AudioBackgroundState::PowerPellet: ss << "PowerPellet"; break;
		case AudioBackgroundState::Retreat: ss << "Retreat"; break;
		default: ss << "Unknown State"; break;
	}

	return ss.str();
}

inline std::string GhostStateToString(GhostState _ghostState)
{
	std::stringstream ss;
	switch (_ghostState) {
		case GhostState::Chase: ss << "Chase"; break;
		case GhostState::Corner: ss << "Corner"; break;
		case GhostState::Frightened: ss << "Frightened"; break;
		case GhostState::GoHome: ss << "GoHome"; break;
		case GhostState::HomeBase: ss << "HomeBase"; break;
		case GhostState::LeaveHome: ss << "LeaveHome"; break;
		case GhostState::EnterHome: ss << "EnterHome"; break;
		default: ss << "Unknown State"; break;
	}

	return ss.str();
}
