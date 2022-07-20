#include "HexagonPlane.h"
#include "Trig.hpp"

HexagonPlane::HexagonIndexPair HexagonPlane::getHex(const sf::Vector3i& index) const {
	auto it = m_hexMap.find(index);
	return HexagonIndexPair(index, it != m_hexMap.end() ? it->second : nullptr);
}

HexagonPlane::HexagonIndexPair HexagonPlane::getNearestHex(const sf::Vector2f& point, float maximumDistance, bool checkCollision) const {
	// TODO: Investigate more efficient solutions
	
	// All distance units are squared to avoid square rooting. Adjust for that reason
	maximumDistance *= maximumDistance;

	float shortestDist = -1.0f;
	HexagonIndexPair ret;
	
	for (auto& i : m_hexMap) {
		// Distance between position and point, as a vector
		sf::Vector2f distVec = i.second->getPosition() - point;
		distVec.x *= distVec.x;
		distVec.y *= distVec.y;

		// `distSqr` is the distance between position and `point`, squared (for performance)
		float distSqr = distVec.x + distVec.y;

		if ((distSqr < maximumDistance || maximumDistance == 0.0f) &&
			(distSqr < shortestDist || shortestDist == -1.0f))
		{
			shortestDist = distSqr;
			ret = HexagonIndexPair(i.first, i.second);
		}
	}

	// Check collision if necessary. Return empty if there was no candidate or the collision check failed
	if (ret.hexagon != nullptr && checkCollision && !ret.hexagon->collidePoint(point)) return HexagonIndexPair();

	return ret;
}

sf::Vector2f HexagonPlane::getPosition(const sf::Vector3i& index) const {
	/*
	// x position = -3/4 * z * hexagon width
	z is bound to the x axis, meaning that each increment corresponds to a full x distance from the center. \
	The x distance between two horizontally adjacent hexagons is one full size, plus \
	one half size (because the hexagons tile diagonally). The negative factor reflects the fact that \
	the z index counts backwards as you go further right
	// y position = (y + z/2) * -1 * hexagon height
	y is bound to the y axis, meaning that each increment corresponds to a full y distance from the center \
	z is bound to the x axis, but because of the diagonal tiling, this also corresponds to a half y distance \
	The y distance between two vertically adjacent hexagons is exactly one full height (= size * sqrt3). \
	The negative factor reflects the fact that the y index counts backwards as you go further down, and \
	the z index counts backwards as you go further right
	*/
	return sf::Vector2f(
		(float)index.z * -0.75f * Hexagon::width(m_hexSize),
		((float)index.y + (float)index.z * 0.5f) * -1.0f * Hexagon::height(m_hexSize)
	);
}

const HexagonPlane::NeighborGroup HexagonPlane::getNeighbors(const sf::Vector3i& index) const {
	NeighborGroup group;
	group.centre = getHex(index);

	for (int i = -1; i < 2; i++) { // equal to current z when xyz = 0,0,0
		int z = i;
		for (int j = 0; j < 2; j++) {
			int y = 0;
			if (i == 0) { y += (2 * j) - 1; } // aka -1 then 1
			else { y += -z + (i * j); } // aka -1 -> 0 and 1 -> 0

			int x = -y - z;

			int iteration = (i + 1) * 2 + j;

			group.neighbors[iteration] = getHex(sf::Vector3i(x + index.x, y + index.y, z + index.z));
		}
	}

	return group;
}

bool HexagonPlane::hasNeighbor(const sf::Vector3i& index) const {
	NeighborGroup group = getNeighbors(index);
	for (unsigned int i = 0; i < 6; i++) if (group.neighbors[i].hexagon != nullptr) return true;
	return false;
}

HexagonPlane::HexagonIndexPair HexagonPlane::getIndex(const sf::Vector2f& position) const {
	sf::Vector3i index;

	index.z = -position.x / (0.75f * Hexagon::width(m_hexSize));
	index.y = -(position.y / (Hexagon::height(m_hexSize)) + (float)index.z * 0.5f);
	index.x = -index.z - index.y;

	return getHex(index);
}

HexagonPlane::HexagonIndexPair HexagonPlane::getIndex(unsigned int layer, float angle) const {
	if (layer == 0) return getHex(sf::Vector3i(0, 0, 0));

	// `interval` is the angle between each hex in the layer
	double interval = PI_THIRDS * pow(0.5, layer - 1);
	double remainder = std::fmod((double)angle, interval);
	
	// Round down
	angle = angle - remainder;
	// Round up if closer to the next interval
	if (remainder >= (interval * 0.5)) angle += interval;

	// This is the distance from the hexagon's center to (0, 0, 0)
	float dist = Hexagon::height(m_hexSize) * layer;

	// Using sin then cos means that `angle` 0 is set at north (vertically upwards)
	return getIndex(sf::Vector2f(sin(angle) * dist, cos(angle) * dist));
}

unsigned int HexagonPlane::getLayer(sf::Vector3i index, const sf::Vector3i& centre) {
	int layer = 0;

	// Index should be in the coordinate space of `centre`
	index = index - centre;

	// A layer can be determined by the sum of positive values in an index (given that x + y + z = 0)
	if (index.x > 0) layer += index.x;
	if (index.y > 0) layer += index.y;
	if (index.z > 0) layer += index.z;

	return layer;
}

HexagonPlane::HexagonIndexPair HexagonPlane::setHex(const sf::Vector3i& index, SPHexagon hexagon) {
	HexagonIndexPair ret;
	ret.index = index;
	
	if (hexagon == nullptr) {
		if (m_factory) hexagon = m_factory();
		else return ret;
	}

	auto it = m_hexMap.find(index);

	// Replace the currently set hex if one exists, otherwise add it
	if (it != m_hexMap.end()) ret.hexagon = it->second = hexagon;
	else m_hexMap.emplace(std::make_pair(index, ret.hexagon = hexagon));

	if (ret.hexagon != nullptr) hexagon->setPosition(getPosition(index));

	return ret;
}

void HexagonPlane::eraseHex(const sf::Vector3i& index) {
	auto it = m_hexMap.find(index);
	if (it != m_hexMap.end()) m_hexMap.erase(it);
}

bool HexagonPlane::Vector3iComparator::operator()(const sf::Vector3i& a, const sf::Vector3i& b) const {
	if (a.z < b.z) return true;
	else if (a.z > b.z) return false;

	if (a.y < b.y) return true;
	else if (a.y > b.y) return false;

	if (a.x < b.x) return true;
	return false;
}