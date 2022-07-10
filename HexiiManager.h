#pragma once

#include "HexagonCluster.h"
#include "NumberDisplay.h"
#include "HexiiUpgradeOverlay.h"

// Singleton class responsible for the gameplay functionality of the Hexii
class HexiiManager
{
private:
	/// ** Structs & Definitions ** ///

	using HexagonIndexPair = HexagonCluster::HexagonIndexPair;
	using NeighborGroup = HexagonCluster::NeighborGroup;
public:
	static HexiiManager* instance();

	/// ** Public member functions **

	/// Events

	// Needs to update the animations for hovering over hexii
	void onMouseMove(sf::Event evnt);
	// Needs to store the hexagon being clicked to validate when released
	void onMouseClick(sf::Event evnt);
	// Needs to process a hex click
	void onMouseReleased(sf::Event evnt);

	void update(float dt);
	void drawGame(sf::RenderWindow& target);
	void drawUI(sf::RenderWindow& target);
private:
	HexiiManager();
	HexiiManager(const HexiiManager&) = delete;
	HexiiManager(HexiiManager&&) = delete;

	static HexiiManager* s_instance;

	/// ** Member functions **

	/// Utility

	// Uses const_cast to gain access to the hexagon stored in an index pair
	// This is completely safe and allows the HexagonPlane to remain mostly const-friendly
	inline static Hexagon* accessHexagon(const HexagonIndexPair& target) { return const_cast<Hexagon*>(target.hexagon); }

	inline bool isNextHexCostAffordable() const { return m_greenMatter.getNumber() >= m_nextHexCost.getNumber(); }

	/// Gameplay

	static void setupHexiiGamepiece(Hexagon* target);
	void processHexClick(HexagonIndexPair target);

	/// UI

	void refreshDisplayNextHexCost();

	/// ** Member variables **

	/// Gameplay variables

	HexagonCluster m_cluster;
	Hexagon m_worldBorder;

	NumberDisplay m_greenMatter;
	NumberDisplay m_nextHexCost;

	/// Event variables

	// The hex that's being moused over
	HexagonIndexPair m_mouseOverHex;
	// The hex closest to the mouse's current position
	HexagonIndexPair m_nearestBorderHex;
	// The last mouse to be clicked
	HexagonIndexPair m_clickedHex;	

	// UI & HUD

	HexiiUpgradeOverlay m_hexiiUpgradeOverlay;
};