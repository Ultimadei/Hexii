#include "HexagonCluster.h"

HexagonCluster::HexagonCluster(Hexagon blueprint, Hexagon borderBlueprint) : 
	m_blueprint(blueprint),
	m_borderBlueprint(borderBlueprint),
	m_mainPlane(blueprint.size()),
	m_borderPlane(blueprint.size())
{
	m_mainPlane.setIndex(blueprint, 0, 0, 0);

	expandBorders(0, 0, 0);
}

HexagonCluster::HexagonIndexPair HexagonCluster::calculateNearestBorder(sf::Vector2f point, float maximumDistance) const {
	return calculateNearest(m_borderPlane, point, maximumDistance);
}

HexagonCluster::HexagonIndexPair HexagonCluster::calculateNearestHexagon(sf::Vector2f point, float maximumDistance) const {
	return calculateNearest(m_mainPlane, point, maximumDistance);
}

HexagonCluster::HexagonIndexPair HexagonCluster::setIndex(int x, int y, int z) {
	return setIndex(sf::Vector3i(x, y, z));
}

HexagonCluster::HexagonIndexPair HexagonCluster::setIndex(const sf::Vector3i& index) {
	HexagonIndexPair retval;
	retval.index = index;

		// Only if there is a border at the given index does it make sense to place a new hexagon there
		if (m_borderPlane.getIndex(index) != nullptr) {
			const Hexagon* result = m_mainPlane.setIndex(m_blueprint, index);
			if (result != nullptr) {
				expandBorders(index);
			}
			retval.hexagon = result;
		}

	return retval;
}

HexagonCluster::HexagonIndexPair HexagonCluster::getIndex(int x, int y, int z) {
	return getIndex(sf::Vector3i(x, y, z));
}

HexagonCluster::HexagonIndexPair HexagonCluster::getIndex(const sf::Vector3i& index) {
	HexagonIndexPair result;

	result.index = index;
	result.hexagon = m_mainPlane.getIndex(index);

	return result;
}

const HexagonCluster::NeighborGroup HexagonCluster::getNeighbors(int x, int y, int z) const {
	return m_mainPlane.getNeighbors(x, y, z);
}

const HexagonCluster::NeighborGroup HexagonCluster::getNeighbors(const sf::Vector3i& index) const {
	return m_mainPlane.getNeighbors(index);
}

const int HexagonCluster::getLayer(int x, int y, int z, int centerX, int centerY, int centerZ) const {
	return getLayer(sf::Vector3i(x, y, z), sf::Vector3i(centerX, centerY, centerZ));
}

const int HexagonCluster::getLayer(sf::Vector3i index, const sf::Vector3i& center) const {
	int layer = 0;

	// This calculates index in terms of a specified center
	index.x -= center.x;
	index.y -= center.y;
	index.z -= center.z;

	// A layer can be determined by the sum of positive values in an index (given that x + y + z = 0)
	// This effectively sums the positive values together
	if (index.x > 0) layer += index.x;
	if (index.y > 0) layer += index.y;
	if (index.z > 0) layer += index.z;

	return layer;
}

void HexagonCluster::setBlueprint(Hexagon blueprint, bool retroactive) {
	m_blueprint = blueprint;

	if (retroactive) {
		for (auto& i : m_mainPlane.accessHexagonMap()) {
			i.second = blueprint;
		}
	}
}

Hexagon HexagonCluster::getBlueprint() const {
	return m_blueprint;
}

void HexagonCluster::setBorderBlueprint(Hexagon blueprint) {
	m_borderBlueprint = blueprint;
	
	for (auto& i : m_borderPlane.accessHexagonMap()) {
		sf::Vector2f preservedPosition = i.second.getPosition();
		i.second = blueprint;
		i.second.setPosition(preservedPosition);
	}
}

Hexagon HexagonCluster::getBorderBlueprint() const {
	return m_borderBlueprint;
}

float HexagonCluster::hexagonSize() const {
	const static float result = m_blueprint.size();
	return result;
}

float HexagonCluster::hexagonWidth() const {
	const static float result = m_blueprint.width();
	return result;
}

float HexagonCluster::hexagonHeight() const {
	const static float result = m_blueprint.height();
	return result;
}

void HexagonCluster::update(float dt) {
	m_mainPlane.update(dt);
	m_borderPlane.update(dt);
}

void HexagonCluster::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.combine(getTransform());

	target.draw(m_mainPlane, states);
	target.draw(m_borderPlane, states);
}

HexagonCluster::HexagonIndexPair HexagonCluster::calculateNearest(const HexagonPlane& plane, sf::Vector2f point, float maximumDistance) const {
	// Adjust `point` into the cluster's space
	point = point - getPosition();
	// All distance units are squared to avoid square rooting. Adjust for that reason
	maximumDistance *= maximumDistance;

	float shortestDistance = -1.0f;
	HexagonIndexPair result;

	for (auto& i : plane.getHexagonMap()) {
		sf::Vector2f distance = i.second.getPosition() - point;
		distance.x *= distance.x;
		distance.y *= distance.y;

		// quickDistance is the distance between position and `point`, squared (for performance)
		float quickDistance = distance.x + distance.y;

		if (quickDistance < (100.0f * 100.0f)) {
			int DBG = 1;
		}

		// If the maximum distance specified is not 0, then the result above must be within the max
		// If the maximum distance specified is exactly 0, it is given as true for now and collision checked later
		bool withinMaximum = maximumDistance == 0.0f ? true : quickDistance < maximumDistance;

		if (
			withinMaximum &&
			(quickDistance < shortestDistance || shortestDistance == -1.0f)
			) {
			shortestDistance = quickDistance;
			result.index = i.first;
			result.hexagon = &i.second;
		}
	}

	if (maximumDistance == 0.0f && result.hexagon != nullptr) {
		// Collision check the nearest
		if (!result.hexagon->collidePoint(point)) { // No collision
			result.index = sf::Vector3i(0, 0, 0);
			result.hexagon = nullptr;
		}
	}

	return result;
}

void HexagonCluster::expandBorders(int x, int y, int z) {
	expandBorders(sf::Vector3i(x, y, z));
}

void HexagonCluster::expandBorders(const sf::Vector3i index) {
	// Erase the current border at index
	m_borderPlane.eraseIndex(index);

	// The neighbors come from the main plane, so that the borders don't overlap with real hexagons
	NeighborGroup borderNeighbors = m_mainPlane.getNeighbors(index);

	for (unsigned int i = 0; i < 6; i++) {
		if (borderNeighbors.neighbors[i].hexagon == nullptr) { // No hexagon at that index
			// Therefore it's a valid border
			m_borderPlane.setIndex(m_borderBlueprint, borderNeighbors.neighbors[i].index, true);
		}
	}
}
