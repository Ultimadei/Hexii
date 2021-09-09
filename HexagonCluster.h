#pragma once

#include "HexagonPlane.h"

// Allows the clustering of multiple hexagons
class HexagonCluster :
	public sf::Drawable, 
	public sf::Transformable {
public:
	using HexagonIndexPair = HexagonPlane::HexagonIndexPair;
	using NeighborGroup = HexagonPlane::NeighborGroup;
public:
	// Construct with a blueprint hexagon. The center and all future hexagons in this cluster will use this blueprint
	HexagonCluster(Hexagon blueprint, Hexagon borderBlueprint);

	// Calculates and returns the HexagonIndexPair of the border nearest the mouse as long as it falls within maximum distance
	// If maximum distance is 0, a collision check will be performed rather than a distance check
	// Returns (0, 0, 0) and nullptr if unsuccessful
	HexagonIndexPair calculateNearestBorder(sf::Vector2f mousePosition, float maximumDistance = 0.0f) const;
	// Calculates and returns the HexagonIndexPair of the hexagon nearest the mouse as long as it falls within maximum distance
	// If maximum distance is 0, a collision check will be performed rather than a distance check
	// Returns (0, 0, 0) and nullptr if unsuccessful
	HexagonIndexPair calculateNearestHexagon(sf::Vector2f mousePosition, float maximumDistance = 0.0f) const;

	// Creates a hexagon at index. Fails if there isn't a border at index (because they must be connected)
	// Returns the HexagonIndexPair of the new hex (with nullptr if unsuccessful)
	HexagonIndexPair setIndex(int x, int y, int z);
	HexagonIndexPair setIndex(const sf::Vector3i& index);

	HexagonIndexPair getIndex(int x, int y, int z);
	HexagonIndexPair getIndex(const sf::Vector3i& index);

	// Returns a group of all the neighbors of the hexagon at the specified index
	const NeighborGroup getNeighbors(int x, int y, int z) const;
	const NeighborGroup getNeighbors(const sf::Vector3i& index) const;

	// Returns how far away index is from a specified center, measured in full hexagons 
	const int getLayer(int x, int y, int z, int centerX = 0, int centerY = 0, int centerZ = 0) const;
	const int getLayer(sf::Vector3i index, const sf::Vector3i& center = sf::Vector3i(0, 0, 0)) const;

	// Updates the blueprint for future hexagons. If retroactive is true, all hexagons are updated
	// Note: The size from the new blueprint is ignored ; size cannot be changed
	void setBlueprint(Hexagon blueprint, bool retroactive = true);
	Hexagon getBlueprint() const;

	// Updates the blueprint for borders
	// Note: The size from the new blueprint is ignored ; size cannot be changed
	void setBorderBlueprint(Hexagon blueprint);
	Hexagon getBorderBlueprint() const;

	float hexagonSize() const;
	float hexagonWidth() const;
	float hexagonHeight() const;

	void update(float dt);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	// Calculates the nearest hexagon in a particular plane
	HexagonIndexPair calculateNearest(const HexagonPlane& plane, sf::Vector2f mousePosition, float maximumDistance = 0.0f) const;

	// Erases the existing border at index, and then expands the surrounding border
	void expandBorders(int x, int y, int z);
	void expandBorders(const sf::Vector3i index);

	Hexagon m_blueprint;
	Hexagon m_borderBlueprint;

	HexagonPlane m_mainPlane;
	HexagonPlane m_borderPlane;	
};
