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

	typedef std::map<sf::Vector3i, Hexagon, Vector3iComparator> HexagonMap;
public:
	/// ** Public Structs & Definitions ** ///

	struct HexagonIndexPair {
		sf::Vector3i index;
		const Hexagon* hexagon = nullptr;
	};

	struct NeighborGroup {
		HexagonIndexPair neighbors[6];
	};
public:
	HexagonPlane(float hexSize);

	/// ** Public Member Functions ** ///

	/// Index manipulation

	// Erases the hexagon at the specified index
	// Does nothing if the index has no hexagon already
	void eraseIndex(int x, int y, int z);
	void eraseIndex(const sf::Vector3i& index);

	// Sets the hexagon at the specified index
	// If conditional, the index will only be set as long as it is currently empty
	// Returns a const pointer to the new hexagon (or nullptr if it was unsuccessful)
	const Hexagon* setIndex(Hexagon hexagon, int x, int y, int z, bool conditional = false);
	const Hexagon* setIndex(Hexagon hexagon, const sf::Vector3i& index, bool conditional = false);

	// Returns a const pointer to the hexagon at the specified index
	// Returns nullptr  
	const Hexagon* getIndex(int x, int y, int z) const;
	const Hexagon* getIndex(const sf::Vector3i& index) const;

	/// Utility functions

	// Returns the indices and hexagons surrounding the specified index
	// Neither index, nor any of its neighbors, are required to have a hexagon set
	const NeighborGroup getNeighbors(int x, int y, int z) const;
	const NeighborGroup getNeighbors(const sf::Vector3i& index) const;

	// Get the position of the hexagon at the specified index
	sf::Vector2f getPositionFromIndex(int x, int y, int z) const;
	sf::Vector2f getPositionFromIndex(const sf::Vector3i& index) const;
	// Get the index which corresponds to the specified position
	// Position is assumed to be valid in this plane, otherwise the behavior is undefined
	sf::Vector3i getIndexFromPosition(const sf::Vector2f& position) const;

	/// Member variable setters & getters

	const HexagonMap& getHexagonMap() const;
	// Returns a non-const reference to the hexagon map
	HexagonMap& accessHexagonMap();

	/// Per-frame functions

	void update(float dt);
protected:
	/// ** Protected Member Functions ** ///

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	/// ** Member variables ** ///

	const float m_hexSize;

	HexagonMap m_hexagonMap;
};