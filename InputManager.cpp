#include "InputManager.h"
#include "DisplayManager.h"

InputManager* InputManager::s_instance = nullptr;

InputManager::InputManager() {}

InputManager* InputManager::instance() {
    if (InputManager::s_instance == nullptr) {
        InputManager::s_instance = new InputManager();
    }

    return InputManager::s_instance;
}

void InputManager::addEventCallback(sf::Event::EventType type, EventCallback callback) {
    auto it = m_eventCallbacks.find(type);
    if (it == m_eventCallbacks.end()) { // There are currently no callbacks registered to this event type
        std::vector<EventCallback> callbacks;
        callbacks.push_back(callback);

        m_eventCallbacks.emplace(std::make_pair(type, callbacks));
    }
    else {
        it->second.push_back(callback);
    }
}

void InputManager::processInput(sf::Event& evnt) {
    switch (evnt.type) {
    case sf::Event::MouseMoved:
        // If the right mouse button is being held down, pan the camera as the mouse moves
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            DisplayManager::instance()->pan(sf::Vector2f(m_lastMousePosition.x - evnt.mouseMove.x, m_lastMousePosition.y - evnt.mouseMove.y), 3.0f);
        }
        
        m_lastMousePosition = sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y);
        break;
    case sf::Event::MouseWheelScrolled:
        // TODO: Hardcoded min/max zoom values should be made configurable
        DisplayManager::instance()->zoom(evnt.mouseWheelScroll.delta * -0.1f, 0.5f, 3.0f);
        break;
    }

    // Now the callbacks
    auto it = m_eventCallbacks.find(evnt.type);
    if (it != m_eventCallbacks.end()) { // One or more callbacks are registered to this event type
        for (EventCallback callback : it->second) {
            callback(evnt);
        }
    }
}