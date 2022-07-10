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
	inline static sf::Vector2i worldToScreen(const sf::Vector2f& point) { return getWindow()->mapCoordsToPixel(point, instance()->m_gameView); }

	/// ** Public member functions **

	/// View modifiers

	// Pans over to `offset` (measured from the origin)
	// if `maxFactor` is non-zero, anything above that many multiples of each axis is clamped (i.e N full screen widths / heights)
	void setViewPan(sf::Vector2f offset, float maxFactor = 0.0f);
	// Sets the zoom factor to `zoom` (note: 1.0 is default)
	// if max is non-zero and outside of (`min`, `max`), zoom is clamped
	void setViewZoom(float zoom, float min = 0.0f, float max = 0.0f);

	// Pans by `offset` (measured from the view's current position)
	// if `maxFactor` is non-zero, anything above that many multiples of each axis is clamped (i.e N full screen widths / heights)
	inline void pan(sf::Vector2f offset, float maxFactor = 0.0f) { setViewPan(m_gameViewPan + offset, maxFactor); }
	// Adds `zoom` to the existing zoom factor (note: multiplicative). Clamps if necessary
	inline void zoom(float zoom, float min = 0.0f, float max = 0.0f) { setViewZoom(m_gameViewZoom + zoom, min, max); }

private:
	DisplayManager();
	DisplayManager(const DisplayManager&) = delete;
	DisplayManager(DisplayManager&&) = delete;

	// Resets the view using the current pan and zoom settings
	void refreshView();

	inline void useGameView() { m_window.setView(m_gameView); }
	// Useful for UI
	inline void useDefaultView() { m_window.setView(m_window.getDefaultView()); }

	static DisplayManager* s_instance;

	/// Primary SFML values

	sf::RenderWindow m_window;

	// View values 

	sf::Vector2u m_windowSize;

	sf::View m_gameView;
	float m_gameViewZoom;
	sf::Vector2f m_gameViewPan;
};