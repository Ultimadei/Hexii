#pragma once

#include "HexagonCluster.h"
#include "NumberDisplay.h"
#include "HexiiUpgradeOverlay.h"

// Singleton class responsible for the gameplay functionality of the Hexii (Hexii is the plural of GameHex)
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

	inline bool isNextHexCostAffordable() const { return m_greenMatter.getNumber() >= m_nextHexCost.getNumber(); }

	/// Gameplay

	void processHexYield(Hexagon* target, BigNumber yield);

	Hexagon* generateGameHex();
	Hexagon* generateBorderHex();

	/// UI
	
	/*
		Reasons for refreshing display:
		- Green matter increased => Need to check if it's now affordable
		- Next hex cost changed => Text change may require resetting origin etc
		- Nearest border changed => Position needs to be updated
	*/
	void refreshDisplayNextHexCost(bool greenMatterChanged, bool nextHexCostChanged, bool nearestBorderChanged);

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