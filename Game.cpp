#include "Game.h"
#include "Trig.hpp"

Game::Game() {
    // Setup the DisplayManager
    m_displayManager = DisplayManager::instance();

    // Setup the InputManager
    m_inputManager = InputManager::instance();
    m_inputManager->addEventCallback(sf::Event::EventType::MouseMoved, [](sf::Event evnt) {
        HexiiManager::instance()->onMouseMove(evnt);
        });

    m_inputManager->addEventCallback(sf::Event::EventType::MouseButtonPressed, [](sf::Event evnt) {
        HexiiManager::instance()->onMouseClick(evnt);
        });

    m_inputManager->addEventCallback(sf::Event::EventType::MouseButtonReleased, [](sf::Event evnt) {
        HexiiManager::instance()->onMouseReleased(evnt);
        });

    // Setup the HexiiManager
    m_hexiiManager = HexiiManager::instance();

    m_window = &m_displayManager->m_window;
}

void Game::run() {
    while (m_window->isOpen()) {
        processInput();

        update();

        draw();
    }
}

void Game::processInput() {
    sf::Event evnt;
    while (m_window->pollEvent(evnt)) {
        // First handle the generic stuff
        switch (evnt.type) {
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::Resized:
            m_displayManager->m_windowSize = sf::Vector2u(evnt.size.width, evnt.size.height);
            m_displayManager->refreshView();
            break;
        }

        // Now forward to the InputManager
        m_inputManager->processInput(evnt);
    }
}

void Game::update() {
    float dt = m_displayManager->getTargetFPS() * m_fpsClock.restart().asSeconds();

    m_hexiiManager->update(dt);    
}

void Game::draw() {
    m_window->clear(sf::Color(200, 200, 200, 255));

    m_displayManager->useGameView();

    m_hexiiManager->drawGame(*m_window);

    /// UI

    m_displayManager->useDefaultView();

    m_hexiiManager->drawUI(*m_window);

    /// Display

    m_window->display();
}