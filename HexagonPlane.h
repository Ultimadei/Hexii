#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "Hexagon.h"

// A transformed plane for hexagons 
class HexagonPlane :
	public sf::Drawable {
private:
	/// ** Private Structs & Definitions ** ///

	struct Vector3iComparator {
		bool operator()(const sf::Vector3i& a, const sf::Vector3i& b) const {
			if (a.z < b.z) {
				return true;
			}
			else if (a.z > b.z) {
				return false;
			}
			if (a.y < b.y) {
				return true;
			}
			else if (a.y > b.y) {
				return false;
			}
			if (a.x < b.x) {
				return true;
			}
			return false;
		}
	};

	typedef std::map<sf::Vector3i, Hexagon*, Vector3iComparator> HexagonMap;
public:
	/// ** Public Structs & Definitions ** ///

	struct HexagonIndexPair {
		sf::Vector3i index;
		Hexagon* hexagon = nullptr;
	};

	struct NeighborGroup {
		HexagonIndexPair centre;
		HexagonIndexPair neighbors[6];
	};
public:
	HexagonPlane(float hexSize);

	/// Interfacing

	/*
	Sets the hexagon at the specified index
	If conditional, the index will only be set as long as it is currently empty
	Returns a HexagonIndexPair containing the new hexagon (or nullptr if it was unsuccessful)
	*/
	HexagonIndexPair setIndex(Hexagon* hexagon, const sf::Vector3i& index, bool conditional = false);
	inline HexagonIndexPair setIndex(Hexagon* hexagon, int x, int y, int z, bool conditional = false) 
		{ return setIndex(hexagon, sf::Vector3i(x, y, z), conditional); }

	// Erases the hexagon at the specified index if one exists
	void eraseIndex(const sf::Vector3i& index);
	inline void eraseIndex(int x, int y, int z) { eraseIndex(sf::Vector3i(x, y, z)); }
	
	// Returns the indices and hexagons surrounding `index`
	// Note: Neither index, nor any of its neighbors, are required to actually have a hexagon
	const NeighborGroup getNeighbors(const sf::Vector3i& index) const;
	inline const NeighborGroup getNeighbors(int x, int y, int z) const { return getNeighbors(sf::Vector3i(x, y, z)); }

	// Get the position of the hexagon at `index`
	sf::Vector2f getPositionFromIndex(const sf::Vector3i& index) const;
	inline sf::Vector2f getPositionFromIndex(int x, int y, int z) const { return getPositionFromIndex(sf::Vector3i(x, y, z)); }

	// Get the index corresponding to `position`
	sf::Vector3i getIndexFromPosition(const sf::Vector2f& position) const;	

	// Returns a HexagonIndexPair containing the hexagon at `index` (or nullptr if none)
	HexagonIndexPair getIndex(const sf::Vector3i& index) const;
	inline HexagonIndexPair getIndex(int x, int y, int z) const { return getIndex(sf::Vector3i(x, y, z)); }

	/// Utility

	// Returns the inner map for easy iteration
	inline HexagonMap& getHexagonMap() { return m_hexagonMap; }
	// Returns the inner map as const
	inline const HexagonMap& hexagonMap() const { return m_hexagonMap; }
	inline float hexagonSize() const { return m_hexSize; }

	void update(float dt);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const float m_hexSize;

	HexagonMap m_hexagonMap;
};