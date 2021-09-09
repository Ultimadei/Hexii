#include "HexagonPlane.h"
#include "Trig.h"

HexagonPlane::HexagonPlane(float hexSize) :
	m_hexSize(hexSize)
{}

void HexagonPlane::eraseIndex(int x, int y, int z) {
	eraseIndex(sf::Vector3i(x, y, z));
}

void HexagonPlane::eraseIndex(const sf::Vector3i& index) {
	auto found = m_hexagonMap.find(index);

	if (found != m_hexagonMap.end()) {
		m_hexagonMap.erase(found);
	}
}

const Hexagon* HexagonPlane::setIndex(Hexagon hexagon, int x, int y, int z, bool conditional) {
	return setIndex(hexagon, sf::Vector3i(x, y, z), conditional);
}

const Hexagon* HexagonPlane::setIndex(Hexagon hexagon, const sf::Vector3i& index, bool conditional) {
	auto found = m_hexagonMap.find(index);

	Hexagon correctedHexagon(m_hexSize);
	correctedHexagon = hexagon;
	correctedHexagon.setPosition(getPositionFromIndex(index));

	if (found != m_hexagonMap.end()) {
		if (!conditional) {
			found->second = correctedHexagon;
			return &found->second;
		}
		// The index already contained a hexagon and conditional was true, so no hexagon was set
	}
	else {
		m_hexagonMap.emplace(std::make_pair(index, correctedHexagon));
		return getIndex(index);
	}

	return nullptr;
}

const Hexagon* HexagonPlane::getIndex(int x, int y, int z) const {
	return getIndex(sf::Vector3i(x, y, z));
}

const Hexagon* HexagonPlane::getIndex(const sf::Vector3i& index) const {
	auto found = m_hexagonMap.find(index);

	if (found != m_hexagonMap.end()) {
		return &found->second;
	}
	return nullptr;
}

const HexagonPlane::NeighborGroup HexagonPlane::getNeighbors(int x, int y, int z) const {
	return getNeighbors(sf::Vector3i(x, y, z));
}

const HexagonPlane::NeighborGroup HexagonPlane::getNeighbors(const sf::Vector3i& index) const {
	NeighborGroup group;

	for (int i = -1; i < 2; i++) { // equal to current z when xyz = 0,0,0
		int currentZ = i;
		for (int j = 0; j < 2; j++) {
			int currentY = 0;
			if (i == 0) { currentY += (2 * j) - 1; } // aka -1 then 1
			else { currentY += -currentZ + (i * j); } // aka -1 -> 0 and 1 -> 0

			int currentX = -currentY - currentZ;

			int iteration = (i + 1) * 2 + j;

			group.neighbors[iteration].index = sf::Vector3i(currentX + index.x, currentY + index.y, currentZ + index.z);
			group.neighbors[iteration].hexagon = getIndex(currentX + index.x, currentY + index.y, currentZ + index.z);
		}
	}

	return group;
}

sf::Vector2f HexagonPlane::getPositionFromIndex(int x, int y, int z) const {
	return getPositionFromIndex(sf::Vector3i(x, y, z));

}

sf::Vector2f HexagonPlane::getPositionFromIndex(const sf::Vector3i& index) const {
	/*
	// x position = -3/4 * z * hexagon width
	z is bound to the x axis, meaning that each increment corresponds to a full x distance from the center. \
	The x distance between two horizontally adjacent hexagons is one full size, plus \
	one half size (because the hexagons tile diagonally). The negative factor reflects the fact that \
	the z index counts backwards as you go further right
	// y position = (y + z/2) * -1 * hexagon height
	y is bound to the y axis, meaning that each increment corresponds to a full y distance from the center \
	z is bound to the x axis, but because of the diagonal tiling, this corresponds to a half y distance \
	The y distance between two vertically adjacent hexagons is exactly one full height (= size * sqrt3). \
	The negative factor reflects the fact that the y index counts backwards as you go further down, and \
	the z index counts backwards as you go further right
	*/
	return sf::Vector2f(
		(float)index.z * -1.5f * m_hexSize,
		((float)index.y + (float)index.z * 0.5f) * -1.0f * m_hexSize * CONSTANT_SQRT3
	);
}

sf::Vector3i HexagonPlane::getIndexFromPosition(const sf::Vector2f& position) const {
	/// !! TODO: DEBUG !! ///

	sf::Vector3i index;

	index.z = -position.x / (m_hexSize * 1.5f);
	index.y = -(position.y / (m_hexSize * CONSTANT_SQRT3) + index.z * 0.5f);
	index.x = -index.z - index.y;

	return index;
}

const HexagonPlane::HexagonMap& HexagonPlane::getHexagonMap() const {
	return m_hexagonMap;
}

HexagonPlane::HexagonMap& HexagonPlane::accessHexagonMap() {
	return m_hexagonMap;
}

void HexagonPlane::update(float dt) {
	for (auto& i : m_hexagonMap) {
		if (i.second.getActive()) { // Checks that i is active (aka should be updated)
			i.second.update(dt);
		}
	}
}

void HexagonPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& i : m_hexagonMap) {
		if (i.second.getActive()) { // Checks that i is active (aka should be drawn)
			target.draw(i.second, states);
		}
	}
}