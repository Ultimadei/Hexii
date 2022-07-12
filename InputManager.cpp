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
        m_lastMousePosition = sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y);
        break;
    case sf::Event::MouseWheelScrolled:
        // TODO: Hardcoded min/max zoom values should be made configurable
        DisplayManager::instance()->zoom(evnt.mouseWheelScroll.delta * -0.1f, 0.5f, 3.0f);
        break;
    }

    // Now the callbacks
    // !!IMPORTANT TODO: Callbacks belonging to objects that have been deleted will lead to undefined behaviour. Fix with smart pointers or similar
    auto it = m_eventCallbacks.find(evnt.type);
    if (it != m_eventCallbacks.end()) { // One or more callbacks are registered to this event type
        for (EventCallback callback : it->second) {
            callback(evnt);
        }
    }
}