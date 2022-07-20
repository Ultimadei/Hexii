#pragma once

#include <functional>
#include <map>

#include "Hexagon.h"

// A transformed plane for hexagons 
class HexagonPlane :
	public sf::Drawable {
public:
	using SPHexagon = Hexagon::SPHexagon;
	typedef std::shared_ptr<HexagonPlane> SPHexagonPlane;

	struct HexagonIndexPair {
		inline HexagonIndexPair(sf::Vector3i index = sf::Vector3i(), SPHexagon hexagon = nullptr) : index(index), hexagon(hexagon) {}

		sf::Vector3i index;
		SPHexagon hexagon = nullptr;
	};

	struct NeighborGroup {
		HexagonIndexPair centre;
		HexagonIndexPair neighbors[6];
	};

protected:
	typedef std::function<SPHexagon()> FactoryFunction;

private:
	struct Vector3iComparator {
		bool operator()(const sf::Vector3i& a, const sf::Vector3i& b) const;
	};

	typedef std::map<sf::Vector3i, SPHexagon, Vector3iComparator> HexagonMap;

public:
	// `size` is used to determine the spacing of hexii (e.g if it matches the hex size then there will be no gaps)
	inline HexagonPlane(float hexSize, FactoryFunction factory = nullptr) : m_hexSize(hexSize), m_factory(factory) {}
	inline virtual ~HexagonPlane() {}

	// Returns a HexagonIndexPair containing the hexagon at `index` (or nullptr if none)
	HexagonIndexPair getHex(const sf::Vector3i& index) const;

	// Calculates the nearest hexagon to `point`
	// If `maximumDistance` > 0, the hex must fall within that distance, and if `checkCollision` is true then `point` must collide with the hex
	HexagonIndexPair getNearestHex(const sf::Vector2f& point, float maximumDistance = 0.0f, bool checkCollision = false) const;

	// Get the position of the hexagon at `index`
	sf::Vector2f getPosition(const sf::Vector3i& index) const;

	// Returns the hexagons surrounding `index`
	// Note: Neither index, nor any of its neighbors, are required to actually have a hexagon set
	const NeighborGroup getNeighbors(const sf::Vector3i& index) const;

	// Checks if there is at least one hex adjacent to `index` (Note: there does not need to be a hex set at `index` itself)
	bool hasNeighbor(const sf::Vector3i& index) const;

	// Get the index corresponding to `position`
	HexagonIndexPair getIndex(const sf::Vector2f& position) const;
	// Gets the hexagon index pair given a layer and an angle in radians measured from north (vertically upwards)
	HexagonIndexPair getIndex(unsigned int layer, float angle) const;

	// Returns how far away `index` is from a specified centre, measured in full hexagons 
	static unsigned int getLayer(sf::Vector3i index, const sf::Vector3i& centre = sf::Vector3i(0, 0, 0));

	// Sets the hexagon at the specified index and returns a HexagonIndexPair containing the new hexagon (or nullptr if it was unsuccessful)
	// If `hexagon` is set to nullptr, this plane's blueprint will be used if possible. Fails otherwise (to remove a hex, use `eraseHex(...)`)
	virtual HexagonIndexPair setHex(const sf::Vector3i& index, SPHexagon hexagon = nullptr);

	// Erases the hexagon at the specified index if one exists
	void eraseHex(const sf::Vector3i& index);

	// Updates the blueprint for future hexagons
	inline void setFactory(FactoryFunction blueprint) { m_factory = blueprint; }

	// Returns the inner map for easy iteration
	inline HexagonMap& getHexMap() { return m_hexMap; }
	// Returns the inner map as const
	inline const HexagonMap& hexMap() const { return m_hexMap; }

	inline float hexSize() const { return m_hexSize; }
	inline float hexWidth() const { return Hexagon::width(m_hexSize); }
	inline float hexHeight() const { return Hexagon::height(m_hexSize); }

	inline void update(float dt) { for (auto& i : m_hexMap) if (i.second->active) i.second->update(dt); }

protected:
	inline virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
		{ for (auto& i : m_hexMap) if (i.second->active) target.draw(*i.second, states); }

	float m_hexSize;
	HexagonMap m_hexMap;
	FactoryFunction m_factory;
};