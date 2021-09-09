#pragma once

#include <list>

#include "HexagonCluster.h"
#include "NumberDisplay.h"
#include "HexiiUpgradeOverlay.h"

class Game {
private:
	/// ** Structs & Definitions ** ///
	using HexagonIndexPair = HexagonCluster::HexagonIndexPair;
	using NeighborGroup = HexagonCluster::NeighborGroup;
public:
	Game();

	void run();
private:
	struct InputStore {
		sf::Vector2f mousePosition;
		HexagonIndexPair collidingHexagon;
		HexagonIndexPair nearestBorder;
	};
private:
	/// ** Member functions ** ///

	/// Refresh functions

	// Refreshes the nextHexCostDisplay text
	void refreshDisplayNextHexCost();

	// Refreshes the current stored border and colliding hexagon
	void refreshStoredHexagons();
	// Refreshes the store for the hexagon the mouse is currently colliding with
	void refreshStoredCollidingHexagon();
	// Refreshes the store for the border nearest the mouse
	void refreshStoredNearestBorder();
	// Refreshes the view according to the zoom factor and panning
	// If use is true, the main view will be set as the active view for window
	void refreshMainView(bool use = true);

	/// Utility functions

	// Returns true if the next hex cost is affordable aka <= current green matter
	bool getNextHexCostAffordable() const;

	// Returns the mouse position in world coordinates
	sf::Vector2f getWorldMousePosition() const;

	// Uses const_cast to gain access to the hexagon stored in an index pair
	// This is completely safe and allows the HexagonPlane to remain mostly const-friendly
	Hexagon* accessHexagon(const HexagonIndexPair& target) const;

	// Sets up gameplay relevant information for a hex
	void setupHexiiGamepiece(Hexagon* target) const;

	/// Gameplay relevant

	// Purchases the next hex, if able
	void purchaseHex(HexagonCluster::HexagonIndexPair target);

	// Begin the initial Hexii click
	void processHexClick(HexagonIndexPair target);

	/// Per-frame functions

	void processInput();
	void update();
	void draw();

	/// ** Member variables ** ///

	/// Primary SFML values

	sf::RenderWindow m_window;
	sf::Clock m_fpsClock;

	// View values 

	const float m_mainViewSize;
	// Can pan up to this many mainViewSizes in both axes
	const float m_mainViewMaximumPanFactor;
	sf::View m_mainView;
	float m_mainViewZoom;
	sf::Vector2f m_mainViewPan;

	/// Stored values

	// Stores data for use between processInput cycles
	InputStore m_currentStore;
	InputStore m_mousePressStore;

	/// Gameplay relevant

	HexagonCluster m_hexagonCluster;
	Hexagon m_worldBorder;

	/// UI 
	
	NumberDisplay m_greenMatter;
	NumberDisplay m_nextHexCost;

	HexiiUpgradeOverlay m_hexiiUpgradeOverlay;
};