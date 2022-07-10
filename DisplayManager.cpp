#include "DisplayManager.h"
#include "InputManager.h"

DisplayManager* DisplayManager::s_instance = nullptr;

// TODO: In future these settings want to come from a config file
DisplayManager::DisplayManager() :
    m_windowSize(960, 600),
    m_window(sf::VideoMode(960, 600), "Hexii"),
    m_gameViewZoom(1.0f),
    m_gameViewPan(0.0f, 0.0f)
{
    m_window.setFramerateLimit(60);
}

DisplayManager* DisplayManager::instance()
{
    if (DisplayManager::s_instance == nullptr) {
        DisplayManager::s_instance = new DisplayManager();
    }

    return DisplayManager::s_instance;
}

void DisplayManager::setViewPan(sf::Vector2f offset, float maxFactor) {
    if (maxFactor != 0.0f) { // Clamping is enabled
        float limitX = maxFactor * offset.x;
        float limitY = maxFactor * offset.y;

        if (offset.x > limitX) offset.x = limitX;
        else if (offset.x < -limitX) offset.x = -limitX;

        if (offset.y > limitY) offset.y = limitY;
        else if (offset.y < -limitY) offset.y = -limitY;
    }

    m_gameViewPan = offset;
    refreshView();
}

void DisplayManager::setViewZoom(float zoom, float min, float max) {
    if (min >= 0.0f && max != 0.0f && min < max) { // Clamping is enabled
        if (zoom > max) zoom = max;
        else if (zoom < min) zoom = min;
    }

    m_gameViewZoom = zoom;
    refreshView();
}

void DisplayManager::refreshView() {
    m_gameView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_windowSize.x * m_gameViewZoom, m_windowSize.y * m_gameViewZoom));
    m_gameView.move(m_gameViewPan);
}