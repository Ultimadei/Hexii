#include "DisplayManager.h"
#include "InputManager.h"

DisplayManager* DisplayManager::s_instance = nullptr;

// TODO: In future these settings want to come from a config file
DisplayManager::DisplayManager() :
    m_targetFPS(60),
    m_windowSize(960, 600),
    m_window(sf::VideoMode(960, 600), "Hexii"),
    m_gameViewZoom(1.0f)
{
    m_window.setFramerateLimit(m_targetFPS);

    refreshView();
}

DisplayManager* DisplayManager::instance() {
    if (DisplayManager::s_instance == nullptr) DisplayManager::s_instance = new DisplayManager();
    return DisplayManager::s_instance;
}

sf::Vector2i DisplayManager::worldToScreen(const sf::Vector2f& point, bool flipY) {
    sf::Vector2i ret = getWindow()->mapCoordsToPixel(point, instance()->m_gameView);
    if (flipY) ret.y = DisplayManager::getWindow()->getSize().y - ret.y;
    return ret;
}

void DisplayManager::setViewZoom(float zoom, float min, float max) {
    if (min >= 0.0f && max != 0.0f && min < max) { // Clamping is enabled
        if (zoom > max) zoom = max;
        else if (zoom < min) zoom = min;
    }

    m_gameViewZoom = zoom;
    refreshView();
}