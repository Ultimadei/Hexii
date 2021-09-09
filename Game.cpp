#include "Game.h"
#include "Trig.h"
#include "ResourceManager.h"

#include "ColorAnimation.h"

Game::Game() :
	m_window(sf::VideoMode(960, 600), "Hexii"),
    m_mainViewSize(1000.0f),
    m_mainViewMaximumPanFactor(1.5f),
    m_mainViewZoom(1.0f),
	m_hexagonCluster(Hexagon(100.0f), Hexagon(100.0f)),
    m_worldBorder(927.63f),
    m_nextHexCost(6)
{
    m_window.setFramerateLimit(60);

    // Fixup the cluster blueprint
    Hexagon clusterBlueprint = m_hexagonCluster.getBlueprint();
    clusterBlueprint.setFillColor(sf::Color(25, 255, 25, 255));
    clusterBlueprint.setOutlineThickness(5.0f);
    clusterBlueprint.setOutlineColor(sf::Color(127, 0, 110, 255));
    m_hexagonCluster.setBlueprint(clusterBlueprint);
    setupHexiiGamepiece(accessHexagon(m_hexagonCluster.getIndex(0, 0, 0)));

    // Fixup the cluster border blueprint
    Hexagon clusterBorderBlueprint = m_hexagonCluster.getBorderBlueprint();
    clusterBorderBlueprint.setFillColor(sf::Color::Transparent);
    clusterBorderBlueprint.setOutlineThickness(clusterBorderBlueprint.size() * -0.15f);
    clusterBorderBlueprint.setOutlineColor(sf::Color(125, 5, 225, 255));
    clusterBorderBlueprint.setActive(false);
    m_hexagonCluster.setBorderBlueprint(clusterBorderBlueprint);

    // Set the world border transformation
    m_worldBorder.setRotation(30.0f);

    // Fixup the world border 
    m_worldBorder.setOutlineThickness(60.0f);
    m_worldBorder.setOutlineColor(sf::Color(0, 0, 0, 225));
    m_worldBorder.setFillColor(sf::Color::Transparent);

    // Fixup the green matter position and display
    m_greenMatter.setPosition(25.0f, 50.0f);
    m_greenMatter.setCharacterSize(70);
    m_greenMatter.setSprite("greenMatter", 0.25f);

    // Fixup the next hex cost display
    m_nextHexCost.setFont(*ResourceManager::getFont("cour"));
    m_nextHexCost.setCharacterSize(60);

    // TODO
    m_hexiiUpgradeOverlay.setBasePosition(m_hexagonCluster.getPosition());

    refreshDisplayNextHexCost();
    refreshMainView();
}

void Game::run() {
    while (m_window.isOpen()) {
        processInput();

        update();

        draw();
    }
}

void Game::refreshDisplayNextHexCost() {
    if (getNextHexCostAffordable()) { // If the next hex is affordable
        m_nextHexCost.setFillColor(sf::Color(30, 255, 30, 255)); // Green
    }
    else {
        m_nextHexCost.setFillColor(sf::Color(127, 0, 0, 255)); // Dark red
    }

    const sf::FloatRect displayBounds = m_nextHexCost.getLocalBounds();

    // Set the display's origin to its local center. This has the effect of centering the text
    m_nextHexCost.setOrigin(
        displayBounds.left + displayBounds.width * 0.5f,
        displayBounds.top + displayBounds.height * 0.5f
    );

    // Scale the display down (or up) to a maximum size
    const float maxWidth = m_hexagonCluster.hexagonWidth() * 0.7f;
    const float maxHeight = m_hexagonCluster.hexagonHeight() * 0.4f;
    float xScaleFactor = maxWidth / displayBounds.width;
    float yScaleFactor = maxHeight / displayBounds.height;
    // Use the smallest of the above for both x and y
    float scaleFactor = xScaleFactor < yScaleFactor ? xScaleFactor : yScaleFactor;

    m_nextHexCost.setScale(scaleFactor, scaleFactor);
}

void Game::refreshStoredHexagons() {
    refreshStoredCollidingHexagon();
    refreshStoredNearestBorder();
}

void Game::refreshStoredCollidingHexagon() {
    HexagonIndexPair collidingHexagon = m_hexagonCluster.calculateNearestHexagon(
        m_currentStore.mousePosition,
        0.0f
    );

    // If the hexagon has not changed, nothing needs to be refreshd
    if (m_currentStore.collidingHexagon.hexagon == collidingHexagon.hexagon) { return; }

    Hexagon* previous = accessHexagon(m_currentStore.collidingHexagon);
    if (previous != nullptr) {
        previous->switchAnimation(
            "hover.color", "_reverse_",
            AnimationParent::SwitchMode::ACTIVATE_IMMITATE_INVERSE_OFFSET, true,
            0.5f, 0.0f
        );
    }

    Hexagon* current = accessHexagon(collidingHexagon);
    if (current != nullptr) {
        current->switchAnimation(
            "hover.color", "_default_",
            AnimationParent::SwitchMode::ACTIVATE, true,
            0.5f, 0.0f
        );
    }

    // Store the new collision
    m_currentStore.collidingHexagon = collidingHexagon;
}

void Game::refreshStoredNearestBorder() {
    HexagonIndexPair nearestBorder;

    if (!m_hexiiUpgradeOverlay.getActive()) {
        nearestBorder = m_hexagonCluster.calculateNearestBorder(
            m_currentStore.mousePosition,
            m_hexagonCluster.hexagonSize() * 1.25f
        );
    }

    // If the border has not changed, nothing needs to be refreshd
    if (m_currentStore.nearestBorder.hexagon == nearestBorder.hexagon) { return; }

    Hexagon* previous = accessHexagon(m_currentStore.nearestBorder);
    if (previous != nullptr) {
        previous->setActive(false);
    }

    Hexagon* current = accessHexagon(nearestBorder);
    if (current != nullptr) {
        /// Move the next hex cost to the border's position

        // Position the display within the nearest border hexagon
        m_nextHexCost.setPosition(current->getPosition());
        // Move the display into the coordinate space of the cluster (where the hexagon would be)
        m_nextHexCost.move(m_hexagonCluster.getPosition());
    }

    // Store the new nearest
    m_currentStore.nearestBorder = nearestBorder;
}

void Game::refreshMainView(bool use) {
    const sf::Vector2u screenSize = m_window.getSize();
    const float ratio = (float)screenSize.x / (float)screenSize.y;
    const float maximumPanDistance = m_mainViewMaximumPanFactor * m_mainViewSize;

    if (m_mainViewPan.x > maximumPanDistance) { m_mainViewPan.x = maximumPanDistance; }
    else if (m_mainViewPan.x < -maximumPanDistance) { m_mainViewPan.x = -maximumPanDistance; }

    if (m_mainViewPan.y > maximumPanDistance) { m_mainViewPan.y = maximumPanDistance; }
    else if (m_mainViewPan.y < -maximumPanDistance) { m_mainViewPan.y = -maximumPanDistance; }

    m_mainView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(ratio * m_mainViewSize, m_mainViewSize));
    m_mainView.zoom(m_mainViewZoom);
    m_mainView.move(m_mainViewPan);

    if (use) {
        m_window.setView(m_mainView);
    }
}

bool Game::getNextHexCostAffordable() const {
    return m_greenMatter.getNumber() >= m_nextHexCost.getNumber();
}

sf::Vector2f Game::getWorldMousePosition() const {
    return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
}

Hexagon* Game::accessHexagon(const HexagonIndexPair& target) const {
    return const_cast<Hexagon*>(target.hexagon);
}

void Game::setupHexiiGamepiece(Hexagon* target) const {
    /// Setup animations

    target->setAnimation("hover.color", new ColorAnimation(
        target, sf::Color(25, 255, 25, 255), sf::Color(25, 255, 200, 255),
        BezierCurvePresets::linear, 0.0f
    ), false, "_default_");

    target->setAnimation("hover.color", new ColorAnimation(
        target, sf::Color(25, 255, 200, 255), sf::Color(25, 255, 25, 255),
        BezierCurvePresets::linear, 0.0f
    ), false, "_reverse_");

    /// Setup data

    target->accessData();
}

void Game::purchaseHex(HexagonIndexPair target) {
    // Place the hexagon
    HexagonIndexPair result = m_hexagonCluster.setIndex(target.index);

    Hexagon* hex = accessHexagon(result);
    if (hex != nullptr) { // If the hexagon was successfully purchased
        setupHexiiGamepiece(hex); // Initialize the hexagon

        // Deduct the cost
        m_greenMatter -= m_nextHexCost.getNumber();
        // refresh the cost for next time
        m_nextHexCost.multiplyNumber(0.0, true);

        // refresh relevant stores and displays
        refreshDisplayNextHexCost();
        refreshStoredHexagons();
    }
}

void Game::processHexClick(HexagonIndexPair target) {
    if (target.hexagon == nullptr) { return; }

    // refresh relevant numbers and displays
    m_greenMatter += floor(accessHexagon(target)->accessData()->power);
    refreshDisplayNextHexCost();
}

void Game::processInput() {
    // All input processing uses the main view
    m_window.setView(m_mainView);

    sf::Event evnt;
    while (m_window.pollEvent(evnt)) {
        switch (evnt.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::MouseButtonPressed:
            m_mousePressStore.mousePosition = m_currentStore.mousePosition;

            m_mousePressStore.collidingHexagon = m_hexagonCluster.calculateNearestHexagon(
                m_currentStore.mousePosition,
                0.0f
            );
            m_mousePressStore.nearestBorder = m_hexagonCluster.calculateNearestBorder(
                m_currentStore.mousePosition,
                0.0f
            );
            break;
        case sf::Event::MouseMoved:
        {
            sf::Vector2f mousePosition = getWorldMousePosition();

            // If the right mouse button is being held down, pan the camera as the mouse moves
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                m_mainViewPan.x += (mousePosition.x - m_currentStore.mousePosition.x) * -1.0f;
                m_mainViewPan.y += (mousePosition.y - m_currentStore.mousePosition.y) * -1.0f;
            }

            refreshMainView();
            m_currentStore.mousePosition = getWorldMousePosition();
            refreshStoredHexagons();
            break;
        }
        case sf::Event::MouseButtonReleased:
            if (evnt.mouseButton.button == sf::Mouse::Button::Left) { // Left mouse click
                // Verify the mouse is and was inside of the world border
                if (!m_worldBorder.collidePoint(m_currentStore.mousePosition) ||
                    !m_worldBorder.collidePoint(m_mousePressStore.mousePosition)
                ) { break; }

                if (m_hexiiUpgradeOverlay.getActive()) {
                    // TODO: Implement upgrades
                }
                // Verify the mouse was inside the same hexagon when it was first pressed
                else if (
                    m_currentStore.collidingHexagon.hexagon != nullptr &&
                    m_currentStore.collidingHexagon.hexagon == m_mousePressStore.collidingHexagon.hexagon
                ) {
                    // Process the click if it was on a main hex
                    processHexClick(m_currentStore.collidingHexagon);
                }
                // Verify the mouse was inside the same border when it was first pressed
                else if (
                    m_currentStore.nearestBorder.hexagon != nullptr &&
                    m_currentStore.nearestBorder.hexagon == m_mousePressStore.nearestBorder.hexagon &&
                    getNextHexCostAffordable()
                ) {
                    // Check for an actual collision
                    if (m_currentStore.nearestBorder.hexagon->collidePoint(m_currentStore.mousePosition)) {
                        purchaseHex(m_currentStore.nearestBorder);
                    }
                }
            }
            else if (evnt.mouseButton.button == sf::Mouse::Button::Right) { // Right mouse click
                float progress = m_hexiiUpgradeOverlay.getProgress();
                if (progress > 0.4f || progress == 0.0f) {
                    if (m_hexiiUpgradeOverlay.getActive()) {
                        m_hexiiUpgradeOverlay.deactivate();
                    }
                    // Hexii upgrade overlay should be activated
                    // Check that the mouse is being released on the same hexagon it initially clicked
                    else if (
                        m_currentStore.collidingHexagon.hexagon != nullptr &&
                        m_currentStore.collidingHexagon.hexagon == m_mousePressStore.collidingHexagon.hexagon
                    ) {
                        m_hexiiUpgradeOverlay.activate(m_currentStore.collidingHexagon.hexagon, 1);
                    }
                }
            }
            break;
        case sf::Event::MouseWheelScrolled:
            m_mainViewZoom += evnt.mouseWheelScroll.delta * -0.1f;
            if (m_mainViewZoom > 3.0f) { m_mainViewZoom = 3.0f; } // Maximum zoom
            if (m_mainViewZoom < 0.5f) { m_mainViewZoom = 0.5f; } // Minimum zoom

            refreshMainView();
            refreshStoredHexagons();
            break;
        case sf::Event::Resized: 
            refreshMainView();
            break;
        }
    }
}

void Game::update() {
    float dt = 60.0f * m_fpsClock.restart().asSeconds();

    m_hexagonCluster.update(dt);

    Hexagon* nearestBorder = accessHexagon(m_currentStore.nearestBorder);
    if (nearestBorder != nullptr) {
        nearestBorder->setActive(true);
    }
    m_hexiiUpgradeOverlay.update(dt);

    m_greenMatter.update(dt);
}

void Game::draw() {
    m_window.clear(sf::Color(127, 127, 127, 255));

    /// Game pieces

    m_window.setView(m_mainView);

    m_window.draw(m_hexagonCluster);

    // Hexagons can't be purchased while the upgrade overlay is active
    if (!m_hexiiUpgradeOverlay.getActive() && m_currentStore.nearestBorder.hexagon != nullptr) {
        // If a border is being drawn, also display the cost to purchase it
        m_window.draw(m_nextHexCost);
    }

    // Draw the upgrade overlay until it is finished animating, aka progress == 0.0f
    if (m_hexiiUpgradeOverlay.getProgress() > 0.0f) {
        m_window.draw(m_hexiiUpgradeOverlay);
    }
    
    m_window.draw(m_worldBorder);

    /// UI

    m_window.setView(m_window.getDefaultView());

    m_window.draw(m_greenMatter);

    /// Display

    m_window.display();
}