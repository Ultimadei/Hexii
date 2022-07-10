#pragma once

#include <functional>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

// Singleton class responsible for communicating user input to elements
class InputManager
{
private:
	/// ** Structs & Definitions ** ///

	typedef std::function<void(sf::Event)> EventCallback;
public:
	static InputManager* instance();

	void addEventCallback(sf::Event::EventType type, EventCallback callback);

	// Handles events as necessary and then passes them through to the callbacks
	void processInput(sf::Event& evnt);

private:
	InputManager();
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;

	static InputManager* s_instance;

	sf::Vector2i m_lastMousePosition;

	// Each event type can have any number of associated callbacks
	// TODO currently no way of getting rid of a callback
	std::map<sf::Event::EventType, std::vector<EventCallback>> m_eventCallbacks;
};