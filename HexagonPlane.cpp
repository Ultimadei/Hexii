#include "HexagonPlane.h"
#include "Trig.h"

HexagonPlane::HexagonPlane(float hexSize) :
	m_hexSize(hexSize)
{}

void HexagonPlane::eraseIndex(const sf::Vector3i& index) {
	auto it = m_hexagonMap.find(index);

	if (it != m_hexagonMap.end()) {
		m_hexagonMap.erase(it);
	}
}

HexagonPlane::HexagonIndexPair HexagonPlane::setIndex(Hexagon* hexagon, const sf::Vector3i& index, bool conditional) {
	auto it = m_hexagonMap.find(index);

	if (hexagon->size() != m_hexSize) throw new std::exception("Attempted to set a hexagon whose size is inconsistent with the plane");

	HexagonIndexPair ret;
	ret.index = index;

	// If the index already contained a hexagon then only replace it if `conditional` is false
	if (it != m_hexagonMap.end() && !conditional) ret.hexagon = it->second = hexagon;
	else m_hexagonMap.emplace(std::make_pair(index, ret.hexagon = hexagon));

	if (ret.hexagon != nullptr) hexagon->setPosition(getPositionFromIndex(index));

	return ret;
}

HexagonPlane::HexagonIndexPair HexagonPlane::getIndex(const sf::Vector3i& index) const {
	auto it = m_hexagonMap.find(index);

	HexagonIndexPair ret;
	ret.index = index;

	if (it != m_hexagonMap.end()) ret.hexagon = it->second;

	return ret;
}

const HexagonPlane::NeighborGroup HexagonPlane::getNeighbors(const sf::Vector3i& index) const {
	NeighborGroup group;
	group.centre = getIndex(index);

	for (int i = -1; i < 2; i++) { // equal to current z when xyz = 0,0,0
		int z = i;
		for (int j = 0; j < 2; j++) {
			int y = 0;
			if (i == 0) { y += (2 * j) - 1; } // aka -1 then 1
			else { y += -z + (i * j); } // aka -1 -> 0 and 1 -> 0

			int x = -y - z;

			int iteration = (i + 1) * 2 + j;

			group.neighbors[iteration] = getIndex(x + index.x, y + index.y, z + index.z);
		}
	}

	return group;
}

sf::Vector2f HexagonPlane::getPositionFromIndex(const sf::Vector3i& index) const {
	/*
	x position = -3/4 * z * hexagon width
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

void HexagonPlane::update(float dt) {
	for (auto& i : m_hexagonMap) {
		if (i.second->getActive()) i.second->update(dt);
	}
}

void HexagonPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& i : m_hexagonMap) {
		if (i.second->getActive()) target.draw(*i.second, states);
	}
}