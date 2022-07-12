#include "HexiiManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"

#include "GameHex.h"
#include "ColorAnimation.h"

HexiiManager* HexiiManager::s_instance = nullptr;

HexiiManager::HexiiManager() :
    m_cluster(100.0f, [this]() { return generateGameHex(); }, [this]() { return generateBorderHex(); }),
    m_worldBorder(927.63f),
    m_nextHexCost(6)
{
    // Setup the world border 
    m_worldBorder.setRotation(30.0f);
    m_worldBorder.setOutlineThickness(60.0f);
    m_worldBorder.setOutlineColor(sf::Color(0, 0, 0, 225));
    m_worldBorder.setFillColor(sf::Color::Transparent);

    // Setup the green matter position and display
    m_greenMatter.setPosition(25.0f, 50.0f);
    m_greenMatter.setCharacterSize(70);
    m_greenMatter.setSprite("greenMatter", 0.25f);

    // Setup the next hex cost display
    m_nextHexCost.setFont(*ResourceManager::getFont("cour"));
    m_nextHexCost.setCharacterSize(60);

    refreshDisplayNextHexCost(true, true, true);
}

HexiiManager* HexiiManager::instance() {
    if (HexiiManager::s_instance == nullptr) {
        HexiiManager::s_instance = new HexiiManager();
    }

    return HexiiManager::s_instance;
}

void HexiiManager::update(float dt) {
    m_cluster.update(dt);
    m_hexiiUpgradeOverlay.update(dt);
    m_greenMatter.update(dt);
}

void HexiiManager::drawGame(sf::RenderWindow& target) {
    target.draw(m_cluster);

    // Hexagons can't be purchased while the upgrade overlay is active
    // If a border is being drawn, also display the cost to purchase it
    if (!m_hexiiUpgradeOverlay.getActive() && m_nearestBorderHex.hexagon != nullptr) target.draw(m_nextHexCost);

    // Draw the upgrade overlay until it is finished animating, aka progress == 0.0f
    if (m_hexiiUpgradeOverlay.getProgress() > 0.0f) {
        target.draw(m_hexiiUpgradeOverlay);
    }

    target.draw(m_worldBorder);
}

void HexiiManager::drawUI(sf::RenderWindow& target) {
    target.draw(m_greenMatter);
}

void HexiiManager::onMouseMove(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y));

    HexagonIndexPair currentMouseOverHex = m_cluster.calculateNearestHexagon(mousePos);
    HexagonIndexPair currentNearestBorderHex = m_cluster.calculateNearestBorder(mousePos, 100.0f, false);

    // No changes necessary if the mouse is over the same hex
    if (currentMouseOverHex.hexagon != m_mouseOverHex.hexagon) {
        Hexagon* previous = m_mouseOverHex.hexagon;
        Hexagon* current = currentMouseOverHex.hexagon;

        if (previous != nullptr) previous->onMouseExit();
        if (current != nullptr) current->onMouseEnter();

        // Update the store
        m_mouseOverHex = currentMouseOverHex;
    }

    // Similar thing for the border hex
    if (currentNearestBorderHex.hexagon != m_nearestBorderHex.hexagon) {
        Hexagon* previous = m_nearestBorderHex.hexagon;
        Hexagon* current = currentNearestBorderHex.hexagon;

        if (previous != nullptr) previous->setActive(false);
        if (current != nullptr && !m_hexiiUpgradeOverlay.getActive()) current->setActive(true);

        // Update the store
        m_nearestBorderHex = currentNearestBorderHex;

        refreshDisplayNextHexCost(false, false, true);
    }
}

void HexiiManager::onMouseClick(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));

    // Only count clicks that are inside of the world border
    if (m_worldBorder.collidePoint(mousePos)) {
        m_clickedHex = m_cluster.calculateNearestHexagon(mousePos);
        if(m_clickedHex.hexagon != nullptr && evnt.mouseButton.button == sf::Mouse::Button::Left && m_clickedHex.hexagon->collidePoint(mousePos)) m_clickedHex.hexagon->onMouseClick();
    }
    else m_clickedHex.hexagon = nullptr;
}

void HexiiManager::onMouseReleased(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));

    /* What can happen:
        - A game hex is right clicked => Open the upgrade overlay
        - User right clicks while the upgrade overlay is open => Close the upgrade overlay
        - A border hex is clicked => Attempt to purchase it [NOT IMPLEMENTED YET]
    */

    // Right clicking while the upgrade overlay is present should hide the overlay (but only do this if enough of the animation has been shown)
    if (m_hexiiUpgradeOverlay.getActive() && evnt.mouseButton.button == sf::Mouse::Button::Right && m_hexiiUpgradeOverlay.getProgress() > 0.4f)
        return m_hexiiUpgradeOverlay.deactivate();

    // Beyond this point, nothing can happen without there being a clicked hex
    if (m_clickedHex.hexagon != nullptr) m_clickedHex.hexagon->onMouseRelease();
    else return;
    
    // Ignore all inputs outside of the world border
    if (!m_worldBorder.collidePoint(mousePos)) return;

    if (evnt.mouseButton.button == sf::Mouse::Button::Left) { // Left mouse click
        // TODO: Implement hex purchasing
    }
    else if (evnt.mouseButton.button == sf::Mouse::Button::Right) { // Right mouse click
        if (m_clickedHex.hexagon == m_cluster.calculateNearestHexagon(mousePos).hexagon) m_hexiiUpgradeOverlay.activate(m_clickedHex.hexagon, 1);
    }
}

void HexiiManager::processHexYield(Hexagon* target, BigNumber yield) {
    // Award `yield` green matter
    m_greenMatter += floor(yield);

    // Refresh displays
    refreshDisplayNextHexCost(true, false, false);
}

Hexagon* HexiiManager::generateGameHex() {
    GameHex* hex = new GameHex(m_cluster.hexagonSize());

    hex->setFillColor(sf::Color(25, 255, 25, 255));

    hex->setAnimation("hover.color", new ColorAnimation(
        hex, sf::Color(25, 255, 25, 255), sf::Color(25, 255, 200, 255),
        BezierCurvePresets::linear, 0.0f
    ), false, "_default_");

    hex->setAnimation("hover.color", new ColorAnimation(
        hex, sf::Color(25, 255, 200, 255), sf::Color(25, 255, 25, 255),
        BezierCurvePresets::linear, 0.0f
    ), false, "_reverse_");

    hex->setOnYield([this](Hexagon* target, BigNumber yield) { processHexYield(target, yield); });

    return hex;
}

Hexagon* HexiiManager::generateBorderHex() {
    float size = m_cluster.hexagonSize();
    Hexagon* hex = new Hexagon(size);

    hex->setFillColor(sf::Color::Transparent);
    hex->setOutlineThickness(size * -0.15f); // Negative thickness => protrudes inward
    hex->setOutlineColor(sf::Color(125, 5, 225, 255));
    hex->setActive(false);

    return hex;
}

void HexiiManager::refreshDisplayNextHexCost(bool greenMatterChanged, bool nextHexCostChanged, bool nearestBorderChanged) {
    if (greenMatterChanged || nextHexCostChanged) {
        if (isNextHexCostAffordable()) m_nextHexCost.setFillColor(sf::Color(30, 255, 30, 255)); // Green
        else m_nextHexCost.setFillColor(sf::Color(127, 0, 0, 255)); // Dark red
    }

    if (nextHexCostChanged) {
        const sf::FloatRect displayBounds = m_nextHexCost.getLocalBounds();

        // Set the display's origin to its local centre. This has the effect of centering the text
        m_nextHexCost.setOrigin(
            displayBounds.left + displayBounds.width * 0.5f,
            displayBounds.top + displayBounds.height * 0.5f
        );

        // Scale the display down (or up) to a maximum size
        const float maxWidth = m_cluster.hexagonWidth() * 0.7f;
        const float maxHeight = m_cluster.hexagonHeight() * 0.4f;
        float xScaleFactor = maxWidth / displayBounds.width;
        float yScaleFactor = maxHeight / displayBounds.height;

        // Use the smallest of the above for both x and y
        float scaleFactor = xScaleFactor < yScaleFactor ? xScaleFactor : yScaleFactor;

        m_nextHexCost.setScale(scaleFactor, scaleFactor);
    }

    if (nearestBorderChanged) {
        if (m_nearestBorderHex.hexagon != nullptr) m_nextHexCost.setPosition(m_nearestBorderHex.hexagon->getPosition());

    }
}

/*
* TODO: Reimplement
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
*/