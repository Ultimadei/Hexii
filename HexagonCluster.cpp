#include "HexagonCluster.h"

HexagonCluster::HexagonCluster(float hexSize, BlueprintFunction blueprint, BlueprintFunction borderBlueprint) :
	m_mainPlane(hexSize),
	m_borderPlane(hexSize)
{
	setBlueprint(blueprint);
	setBorderBlueprint(borderBlueprint);

	m_mainPlane.setIndex(m_blueprint(), 0, 0, 0);

	expandBorders(0, 0, 0);
}

HexagonCluster::HexagonIndexPair HexagonCluster::setIndex(const sf::Vector3i& index) {
	HexagonIndexPair ret;
	ret.index = index;

	// Only if there is a border at the given index does it make sense to place a new hexagon there
	if (m_borderPlane.getIndex(index).hexagon != nullptr) {
		if ((ret.hexagon = m_mainPlane.setIndex(m_blueprint(), index).hexagon) != nullptr) expandBorders(index);
	}

	return ret;
}

const int HexagonCluster::getLayer(sf::Vector3i index, const sf::Vector3i& centre) const {
	int layer = 0;

	// This calculates index in terms of a specified centre
	index = index - centre;

	// A layer can be determined by the sum of positive values in an index (given that x + y + z = 0)
	if (index.x > 0) layer += index.x;
	if (index.y > 0) layer += index.y;
	if (index.z > 0) layer += index.z;

	return layer;
}

void HexagonCluster::setBorderBlueprint(BlueprintFunction blueprint) {
	m_borderBlueprint = blueprint;
	
	// Existing borders need to be updated
	for (auto& i : m_borderPlane.getHexagonMap()) {
		// Store pos so it doesn't need to be recalculated
		sf::Vector2f pos = i.second->getPosition();
		i.second = blueprint();
		i.second->setPosition(pos);
	}
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

HexagonCluster::HexagonIndexPair HexagonCluster::calculateNearest(const HexagonPlane& plane, sf::Vector2f point, float maximumDistance, bool checkCollision) const {
	// Adjust `point` into the cluster's space
	point = point - getPosition();
	// All distance units are squared to avoid square rooting. Adjust for that reason
	maximumDistance *= maximumDistance;

	float shortestDist = -1.0f;
	HexagonIndexPair result;

	for (auto& i : plane.hexagonMap()) {
		sf::Vector2f dist = i.second->getPosition() - point;
		dist.x *= dist.x;
		dist.y *= dist.y;

		// `distSqr` is the distance between position and `point`, squared (for performance)
		float distSqr = dist.x + dist.y;

		if ((distSqr < maximumDistance || maximumDistance == 0.0f) && 
			(distSqr < shortestDist || shortestDist == -1.0f))
		{
			shortestDist = distSqr;
			result.index = i.first;
			result.hexagon = i.second;
		}
	}

	if (result.hexagon != nullptr && checkCollision) {
		if (!result.hexagon->collidePoint(point)) {
			// There was no collision => unsuccessful
			result.index = sf::Vector3i(0, 0, 0);
			result.hexagon = nullptr;
		}
	}

	return result;
}

void HexagonCluster::expandBorders(const sf::Vector3i index) {
	// Erase the current border at index
	m_borderPlane.eraseIndex(index);

	// The neighbors come from the main plane, so that the borders don't overlap with real hexagons
	NeighborGroup borderNeighbors = m_mainPlane.getNeighbors(index);

	for (unsigned int i = 0; i < 6; i++) {
		if (borderNeighbors.neighbors[i].hexagon == nullptr) { // No hexagon at that index
			// Therefore it's a valid border
			// !!IMPORTANT TODO: Memory leak here. blueprint produces a new hex which may be unused because of `conditional`
			m_borderPlane.setIndex(m_borderBlueprint(), borderNeighbors.neighbors[i].index, true);
		}
	}
}
