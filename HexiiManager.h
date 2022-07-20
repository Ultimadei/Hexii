#pragma once

#include "HexagonCluster.hpp"
#include "NumberHUD.h"
#include "HexiiUpgradeGUI.h"
#include "GameHex.h"

// Singleton class responsible for the gameplay functionality of the Hexii (Hexii is the plural of GameHex)
class HexiiManager
{
private:
	/// ** Structs & Definitions ** ///

	using HexagonIndexPair = HexagonCluster::HexagonIndexPair;
	using NeighborGroup = HexagonCluster::NeighborGroup;

	using SPHexagon = Hexagon::SPHexagon;
	using SPGameHex = GameHex::SPGameHex;
	using SPHexagonPlane = HexagonPlane::SPHexagonPlane;
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
	// Draws all the elements that live in the game view
	void drawGame(sf::RenderWindow& target);
	// Draws the elements that aren't affected by the game view (some but not all UI & HUD elements)
	void drawUI(sf::RenderWindow& target);
private:
	HexiiManager();
	HexiiManager(const HexiiManager&) = delete;
	HexiiManager(HexiiManager&&) = delete;

	static HexiiManager* s_instance;

	/// Gameplay

	void processHexYield(const GameHex& target, BigNumber yield);
	void expandBorders(const sf::Vector3i& at);

	SPHexagon generateGameHex();
	SPHexagon generateBorderHex();

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

	SPHexagonPlane m_plane;
	HexagonCluster m_cluster;
	HexagonCluster m_borderCluster;
	Hexagon m_worldBorder;

	SPBigNumber m_greenMatter;
	SPBigNumber m_nextHexCost;

	/// Event variables

	// The hex that's being moused over
	HexagonIndexPair m_mouseOverHex;
	// The hex closest to the mouse's current position
	HexagonIndexPair m_nearestBorderHex;
	// The last mouse to be clicked
	HexagonIndexPair m_clickedHex;	

	// UI & HUD

	NumberHUD m_greenMatterHUD;
	NumberHUD m_nextHexCostHUD;
	HexiiUpgradeGUI m_hexiiUpgradeGUI;
};