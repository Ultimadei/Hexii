#pragma once

#include <SFML/Graphics.hpp>

// Singleton class responsible for the window, camera and views
class DisplayManager
{
	friend class Game;
public:
	static DisplayManager* instance();

	inline static const sf::RenderWindow* getWindow() { return &s_instance->m_window; }

	/// ** Utility **

	// Converts screen/pixel space coordinates to world space (e.g for mouse position)
	inline static sf::Vector2f screenToWorld(const sf::Vector2i& point) { return getWindow()->mapPixelToCoords(point, instance()->m_gameView); }
	// Converts world space coordinates to screen space (e.g for element position on screen)
	// flipY is necessary if the return value is being used in a shader because OpenGL has an inverted y
	static sf::Vector2i worldToScreen(const sf::Vector2f& point, bool flipY = false);

	inline static unsigned int getTargetFPS() { return instance()->m_targetFPS; }

	/// ** Public member functions **

	/// View modifiers

	// Sets the zoom factor to `zoom` (note: 1.0 is default)
	// if max is non-zero and outside of (`min`, `max`), zoom is clamped
	void setViewZoom(float zoom, float min = 0.0f, float max = 0.0f);

	// Adds `zoom` to the existing zoom factor (note: multiplicative). Clamps if necessary
	inline void zoom(float zoom, float min = 0.0f, float max = 0.0f) { setViewZoom(m_gameViewZoom + zoom, min, max); }

private:
	DisplayManager();
	DisplayManager(const DisplayManager&) = delete;
	DisplayManager(DisplayManager&&) = delete;

	// Resets the view using the current pan and zoom settings
	inline void refreshView()
		{ m_gameView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_windowSize.x * m_gameViewZoom, m_windowSize.y * m_gameViewZoom)); }

	inline void useGameView() { m_window.setView(m_gameView); }
	// Useful for UI
	inline void useDefaultView() { m_window.setView(m_window.getDefaultView()); }

	static DisplayManager* s_instance;

	const unsigned int m_targetFPS;

	/// Primary SFML values

	sf::RenderWindow m_window;

	// View values 

	sf::Vector2u m_windowSize;

	sf::View m_gameView;
	float m_gameViewZoom;
};