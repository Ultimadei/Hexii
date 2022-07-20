#include "HexiiManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"

#include "GameHex.h"
#include "ColorAnimation.hpp"

HexiiManager* HexiiManager::s_instance = nullptr;

HexiiManager::HexiiManager() :
    m_plane(std::make_shared<HexagonPlane>(100.0f)),
    m_cluster(m_plane, [this]() { return generateGameHex(); }),
    m_borderCluster(m_plane, [this]() {return generateBorderHex(); }),
    m_worldBorder(927.63f),
    m_greenMatter(std::make_shared<BigNumber>(0)),
    m_nextHexCost(std::make_shared<BigNumber>(6)),
    m_greenMatterHUD(m_greenMatter),
    m_nextHexCostHUD(m_nextHexCost)
{
    // Setup the first hex
    m_cluster.setHex(sf::Vector3i(0, 0, 0));
    expandBorders(sf::Vector3i(0, 0, 0));

    // Setup the world border 
    m_worldBorder.setRotation(30.0f);
    m_worldBorder.setOutlineThickness(60.0f);
    m_worldBorder.setOutlineColor(sf::Color(0, 0, 0, 225));
    m_worldBorder.setFillColor(sf::Color::Transparent);

    // Setup the green matter position and display
    m_greenMatterHUD.setPosition(25.0f, 50.0f);
    m_greenMatterHUD.getText()->setCharacterSize(70);
    m_greenMatterHUD.setSprite(ResourceManager::getTexture("greenMatter"), 0.25f);

    // Setup the next hex cost display
    m_nextHexCostHUD.getText()->setCharacterSize(60);
    m_nextHexCostHUD.centerText();
    m_nextHexCostHUD.scaleTextToFit(m_plane->hexWidth() * 0.7f, m_plane->hexHeight() * 0.4f);
    m_nextHexCostHUD.active = false;

    refreshDisplayNextHexCost(true, true, true);
}

HexiiManager* HexiiManager::instance() {
    if (HexiiManager::s_instance == nullptr) HexiiManager::s_instance = new HexiiManager();
    return HexiiManager::s_instance;
}

void HexiiManager::update(float dt) {
    m_plane->update(dt);
    m_hexiiUpgradeGUI.update(dt);
    m_greenMatterHUD.update(dt);
    m_nextHexCostHUD.update(dt);
}

void HexiiManager::drawGame(sf::RenderWindow& target) {
    target.draw(*m_plane);

    // Hexagons can't be purchased while the upgrade overlay is active
    // But if a border is being drawn, also display the cost to purchase it
    if (!m_hexiiUpgradeGUI.active && m_nearestBorderHex.hexagon != nullptr) target.draw(m_nextHexCostHUD);

    // Draw the upgrade overlay until it is finished animating, aka progress == 0.0f
    if (m_hexiiUpgradeGUI.active) target.draw(m_hexiiUpgradeGUI);

    target.draw(m_worldBorder);
}

void HexiiManager::drawUI(sf::RenderWindow& target) {
    target.draw(m_greenMatterHUD);
}

void HexiiManager::onMouseMove(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y));

    HexagonIndexPair currentMouseOverHex = m_cluster.getNearestHex(mousePos, 0.0f, true);
    HexagonIndexPair currentNearestBorderHex = m_borderCluster.getNearestHex(mousePos, 100.0f, false);

    // No changes necessary if the mouse is over the same hex
    if (currentMouseOverHex.hexagon != m_mouseOverHex.hexagon) {
        SPHexagon previous = m_mouseOverHex.hexagon;
        SPHexagon current = currentMouseOverHex.hexagon;

        if (previous != nullptr) previous->onMouseExit();
        if (current != nullptr) current->onMouseEnter();

        // Update the store
        m_mouseOverHex = currentMouseOverHex;
    }

    // Similar thing for the border hex
    if (currentNearestBorderHex.hexagon != m_nearestBorderHex.hexagon) {
        SPHexagon previous = m_nearestBorderHex.hexagon;
        SPHexagon current = currentNearestBorderHex.hexagon;

        if (previous != nullptr) previous->active = false;
        if (current != nullptr && !m_hexiiUpgradeGUI.active) current->active = true;

        // Update the store
        m_nearestBorderHex = currentNearestBorderHex;

        refreshDisplayNextHexCost(false, false, true);
    }
}

void HexiiManager::onMouseClick(sf::Event evnt) {
    sf::Vector2f mousePos = DisplayManager::screenToWorld(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));

    // Only count clicks that are inside of the world border
    if (m_worldBorder.collidePoint(mousePos)) {
        m_clickedHex = m_cluster.getNearestHex(mousePos);
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
    if (m_hexiiUpgradeGUI.active && evnt.mouseButton.button == sf::Mouse::Button::Right && m_hexiiUpgradeGUI.getProgress() > 0.4f)
        return m_hexiiUpgradeGUI.deactivate();

    // Beyond this point, nothing can happen without there being a clicked hex
    if (m_clickedHex.hexagon != nullptr) m_clickedHex.hexagon->onMouseRelease();
    else return;
    
    // Ignore all inputs outside of the world border
    if (!m_worldBorder.collidePoint(mousePos)) return;

    if (evnt.mouseButton.button == sf::Mouse::Button::Left) { // Left mouse click
        // TODO: Implement hex purchasing
    }
    else if (evnt.mouseButton.button == sf::Mouse::Button::Right) { // Right mouse click
        if (m_clickedHex.hexagon == m_cluster.getNearestHex(mousePos).hexagon) 
            m_hexiiUpgradeGUI.activate(m_clickedHex.hexagon, 1);

        // TODO: Activating an animation that's already active breaks stuff
    }
}

void HexiiManager::processHexYield(const GameHex& target, BigNumber yield) {
    // Award `yield` green matter
    *m_greenMatter += floor(yield);

    // Refresh displays
    refreshDisplayNextHexCost(true, false, false);
}

void HexiiManager::expandBorders(const sf::Vector3i& at) {
    NeighborGroup group = m_plane->getNeighbors(at);
    for (unsigned int i = 0; i < 6; i++) {
        // If there is no neighbor set then expand the border
        if (group.neighbors[i].hexagon == nullptr) m_borderCluster.setHex(group.neighbors[i].index);
        else if (m_cluster.getHex(group.neighbors[i].index).hexagon == nullptr) continue; // There is a border already so don't bother replacing it
    }
}

HexiiManager::SPHexagon HexiiManager::generateGameHex() {
    SPGameHex hex = std::make_shared<GameHex>(m_plane->hexSize());

    hex->setFillColor(sf::Color(58, 193, 121, 255));

    std::function<void(const ColorAnimation&)> applyHoverColor = [hex](const ColorAnimation& animation) { hex->setFillColor(animation.getOutput()); };

    hex->animator["hoverColor"] = std::make_shared<ColorAnimation>(applyHoverColor, sf::Color(58, 193, 121, 255), sf::Color(58, 240, 121, 255), BezierCurves::linear, 0.4f);
    hex->animator["hoverColorReverse"] = std::make_shared<ColorAnimation>(applyHoverColor, sf::Color(58, 240, 121, 255), sf::Color(58, 193, 121, 255), BezierCurves::linear, 0.5f);

    hex->setOnYield([this](const GameHex& target, BigNumber yield) { processHexYield(target, yield); });

    return hex;
}

HexiiManager::SPHexagon HexiiManager::generateBorderHex() {
    float size = m_plane->hexSize();
    SPHexagon hex = std::make_shared<Hexagon>(size);

    hex->setFillColor(sf::Color::Transparent);
    hex->setOutlineThickness(size * -0.15f); // Negative thickness => protrudes inward
    hex->setOutlineColor(sf::Color(125, 5, 225, 255));
    hex->active = false;

    return hex;
}

void HexiiManager::refreshDisplayNextHexCost(bool greenMatterChanged, bool nextHexCostChanged, bool nearestBorderChanged) {
    if (greenMatterChanged || nextHexCostChanged) {
        if (*m_greenMatter >= *m_nextHexCost) m_nextHexCostHUD.getText()->setFillColor(sf::Color(30, 255, 30, 255)); // Green
        else m_nextHexCostHUD.getText()->setFillColor(sf::Color(127, 0, 0, 255)); // Dark red
    }

    if (nearestBorderChanged) {
        if (m_nearestBorderHex.hexagon != nullptr) {
            m_nextHexCostHUD.setPosition(m_nearestBorderHex.hexagon->getPosition());
            m_nextHexCostHUD.active = true;
        }
        else m_nextHexCostHUD.active = false;
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