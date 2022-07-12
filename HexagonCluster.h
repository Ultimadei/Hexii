#pragma once

#include "HexagonPlane.h"

#include <functional>

// Allows the clustering of multiple hexagons
class HexagonCluster :
	public sf::Drawable, 
	public sf::Transformable {
public:
	using HexagonIndexPair = HexagonPlane::HexagonIndexPair;
	using NeighborGroup = HexagonPlane::NeighborGroup;

	typedef std::function<Hexagon* ()> BlueprintFunction;
public:
	// Construct with a blueprint. The centre and all future hexagons in this cluster will use this blueprint
	HexagonCluster(float hexSize, BlueprintFunction blueprint, BlueprintFunction borderBlueprint);

	// Calculates and returns the HexagonIndexPair of the hexagon nearest `point` as long as it falls within maximum distance (0 for no max)
	// Returns (0, 0, 0) and nullptr if unsuccessful
	inline HexagonIndexPair calculateNearestHexagon(sf::Vector2f point, float maximumDistance = 0.0f, bool checkCollision = true) const
		{ return calculateNearest(m_mainPlane, point, maximumDistance, checkCollision); }

	// Calculates and returns the HexagonIndexPair of the border nearest `point` as long as it falls within maximum distance (0 for no max)
	// Returns (0, 0, 0) and nullptr if unsuccessful
	inline HexagonIndexPair calculateNearestBorder(sf::Vector2f point, float maximumDistance = 0.0f, bool checkCollision = true) const
		{ return calculateNearest(m_borderPlane, point, maximumDistance, checkCollision); }
	

	// Creates a hexagon at `index`. Fails if there isn't a border at `index` (because they must be connected)
	// Returns the HexagonIndexPair of the new hex (with nullptr if unsuccessful)
	HexagonIndexPair setIndex(const sf::Vector3i& index);
	inline HexagonIndexPair setIndex(int x, int y, int z) { return setIndex(sf::Vector3i(x, y, z)); }

	
	// Returns the HexagonIndexPair containing the hex at `index`
	inline HexagonIndexPair getIndex(const sf::Vector3i& index) const { return m_mainPlane.getIndex(index); }
	inline HexagonIndexPair getIndex(int x, int y, int z) const { return getIndex(sf::Vector3i(x, y, z)); }


	// Returns a group of all the neighbors of the hexagon at the specified index
	inline const NeighborGroup getNeighbors(const sf::Vector3i& index) const { return m_mainPlane.getNeighbors(index); }
	inline const NeighborGroup getNeighbors(int x, int y, int z) const { return m_mainPlane.getNeighbors(x, y, z); }
	

	// Returns how far away `index` is from a specified centre, measured in full hexagons 
	const int getLayer(sf::Vector3i index, const sf::Vector3i& centre = sf::Vector3i(0, 0, 0)) const;
	inline const int getLayer(int x, int y, int z, int centreX = 0, int centreY = 0, int centreZ = 0) const 
		{ return getLayer(sf::Vector3i(x, y, z), sf::Vector3i(centreX, centreY, centreZ)); }
	

	// Updates the blueprint for future hexagons
	inline void setBlueprint(BlueprintFunction blueprint) { m_blueprint = blueprint; }
	// Updates the blueprint for borders with immediate retroactive effect
	void setBorderBlueprint(BlueprintFunction blueprint);

	inline float hexagonSize() const { return m_mainPlane.hexagonSize(); }
	inline float hexagonWidth() const { return Hexagon::width(hexagonSize()); }
	inline float hexagonHeight() const { return Hexagon::height(hexagonSize()); }

	void update(float dt);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	// Calculates the nearest hexagon in a particular plane
	HexagonIndexPair calculateNearest(const HexagonPlane& plane, sf::Vector2f point, float maximumDistance = 0.0f, bool checkCollision = false) const;

	// Erases the existing border at index, and then expands the surrounding border
	void expandBorders(const sf::Vector3i index);
	inline void expandBorders(int x, int y, int z) { expandBorders(sf::Vector3i(x, y, z)); }

	BlueprintFunction m_blueprint;
	BlueprintFunction m_borderBlueprint;

	// TODO: Each hexagon cluster should not have its own plane. It should be given one to share
	HexagonPlane m_mainPlane;
	HexagonPlane m_borderPlane;
};