#pragma once

#include "DisplayManager.h"
#include "InputManager.h"
#include "HexiiManager.h"

// Class for the main game loop
class Game {
private:
	/// ** Structs & Definitions ** ///
	
public:
	Game();

	void run();
private:
	void processInput();
	void update();
	void draw();

	/// ** Member variables ** ///

	DisplayManager* m_displayManager;
	InputManager* m_inputManager;
	HexiiManager* m_hexiiManager;

	sf::Clock m_fpsClock;
	// Used for the basic input and draw functions
	sf::RenderWindow* m_window;
};