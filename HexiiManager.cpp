#include "HexiiManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"

#include "ColorAnimation.h"

HexiiManager* HexiiManager::s_instance = nullptr;

HexiiManager::HexiiManager() :
    m_cluster(Hexagon(100.0f), Hexagon(100.0f)),
    m_worldBorder(927.63f),
    m_nextHexCost(6)
{
    // Setup the cluster blueprint
    Hexagon clusterBlueprint = m_cluster.getBlueprint();
    clusterBlueprint.setFillColor(sf::Color(25, 255, 25, 255));
    //clusterBlueprint.setOutlineThickness(5.0f);
    //clusterBlueprint.setOutlineColor(sf::Color(127, 0, 110, 255));
    clusterBlueprint.setShader(ResourceManager::getShader("hex"));
    m_cluster.setBlueprint(clusterBlueprint);
    setupHexiiGamepiece(accessHexagon(m_cluster.getIndex(0, 0, 0)));

    // Setup the cluster border blueprint
    Hexagon clusterBorderBlueprint = m_cluster.getBorderBlueprint();
    clusterBorderBlueprint.setFillColor(sf::Color::Transparent);
    clusterBorderBlueprint.setOutlineThickness(clusterBorderBlueprint.size() * -0.15f);
    clusterBorderBlueprint.setOutlineColor(sf::Color(125, 5, 225, 255));
    clusterBorderBlueprint.setActive(false);
    m_cluster.setBorderBlueprint(clusterBorderBlueprint);

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

    refreshDisplayNextHexCost();
}

HexiiManager* HexiiManager::instance() {
    if (HexiiManager::s_instance == nullptr) {
        HexiiManager::s_instance = new HexiiManager();
    }

    return HexiiManager::s_instance;
}

void HexiiManager::update(float dt) {
    m_cluster.update(dt);

    // TODO: Is this necessary?
    if (m_nearestBorderHex.hexagon != nullptr) accessHexagon(m_nearestBorderHex)->setActive(true);

    m_hexiiUpgradeOverlay.update(dt);
    m_greenMatter.update(dt);
}

void HexiiManager::drawGame(sf::RenderWindow& target) {
    target.draw(m_cluster);

    // Hexagons can't be purchased while the upgrade overlay is active
    // If a border is being drawn, also display the cost to purchase it
    if (!m_hexiiUpgradeOverlay.getActive() && m_nearestBorderHex.hexagon != nullptr) target.draw(m_nextHexCost);
    // Draw the upgrade overlay until it is finished animating, aka progress == 0.0f
    else if (m_hexiiUpgradeOverlay.getProgress() > 0.0f) {
        target.draw(m_hexiiUpgradeOverlay);
    }

    target.draw(m_worldBorder);
}


void HexiiManager::drawUI(sf::RenderWindow& target) {
    target.draw(m_greenMatter);
}

void HexiiManager::setupHexiiGamepiece(Hexagon* target) {
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

void HexiiManager::onMouseMove(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y));

    HexagonIndexPair currentMouseOverHex = m_cluster.calculateNearestHexagon(mousePos);
    HexagonIndexPair currentNearestBorderHex = m_cluster.calculateNearestBorder(mousePos, 50.0f);

    // No changes necessary if the mouse is over the same hex
    if (currentMouseOverHex.hexagon != m_mouseOverHex.hexagon) {
        Hexagon* previous = accessHexagon(m_mouseOverHex);
        Hexagon* current = accessHexagon(currentMouseOverHex);

        if (previous != nullptr) {
            previous->switchAnimation(
                "hover.color", "_reverse_",
                AnimationParent::SwitchMode::ACTIVATE_IMMITATE_INVERSE_OFFSET, true,
                0.5f, 0.0f
            );
        }
        if (current != nullptr) {
            current->switchAnimation(
                "hover.color", "_default_",
                AnimationParent::SwitchMode::ACTIVATE, true,
                0.5f, 0.0f
            );
        }

        // Update the store
        m_mouseOverHex = currentMouseOverHex;
    }

    // Similar thing for the border hex
    if (currentNearestBorderHex.hexagon != m_nearestBorderHex.hexagon) {
        Hexagon* previous = accessHexagon(m_nearestBorderHex);
        Hexagon* current = accessHexagon(currentNearestBorderHex);

        if (previous != nullptr) previous->setActive(false);
        if (current != nullptr) current->setActive(true);

        // Update the store
        m_nearestBorderHex = currentNearestBorderHex;
    }
}

void HexiiManager::onMouseClick(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));

    // Only count clicks that are inside of the world border
    if (m_worldBorder.collidePoint(mousePos)) m_clickedHex = m_cluster.calculateNearestHexagon(mousePos);
    else m_clickedHex.hexagon = nullptr;
}

void HexiiManager::onMouseReleased(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));

    /*  Nothing will happen here if
             - Mouse initially clicked on nothing
             - Mouse is released on nothing
             - Mouse is released while the upgrade overlay is active
             - Mouse is released outside of the hex it initially clicked

            Otherwise, three things can happen:
             - A hex is left clicked => Process the click
             - A hex is right clicked => Open the upgrade overlay
             - A border hex is clicked => Attempt to purchase it [TODO: NOT IMPLEMENTED YET]
    */

    if (m_clickedHex.hexagon == nullptr ||
        !m_worldBorder.collidePoint(mousePos) ||
        m_hexiiUpgradeOverlay.getActive())
        return;

    if (evnt.mouseButton.button == sf::Mouse::Button::Left) { // Left mouse click
        if (m_clickedHex.hexagon == m_cluster.calculateNearestHexagon(mousePos).hexagon) processHexClick(m_clickedHex);
        else {} // TODO: Implement hex purchasing
    }
    else if (evnt.mouseButton.button == sf::Mouse::Button::Right) { // Right mouse click
        if (m_clickedHex.hexagon == m_cluster.calculateNearestHexagon(mousePos).hexagon) m_hexiiUpgradeOverlay.activate(m_clickedHex.hexagon, 1);
    }
}

void HexiiManager::processHexClick(HexagonIndexPair target) {
    // Refresh relevant numbers and displays
    m_greenMatter += floor(accessHexagon(target)->accessData()->power);
    refreshDisplayNextHexCost();
}

void HexiiManager::refreshDisplayNextHexCost() {
    if (isNextHexCostAffordable()) {
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
    const float maxWidth = m_cluster.hexagonWidth() * 0.7f;
    const float maxHeight = m_cluster.hexagonHeight() * 0.4f;
    float xScaleFactor = maxWidth / displayBounds.width;
    float yScaleFactor = maxHeight / displayBounds.height;
    // Use the smallest of the above for both x and y
    float scaleFactor = xScaleFactor < yScaleFactor ? xScaleFactor : yScaleFactor;

    m_nextHexCost.setScale(scaleFactor, scaleFactor);
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