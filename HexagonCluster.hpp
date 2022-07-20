#pragma once

#include "HexagonPlane.h"

#include <functional>

// A subset of the plane; all hexii added to a cluster will be added to the base plane, but not vice versa
class HexagonCluster final :
	public HexagonPlane {	
public:
	inline HexagonCluster(SPHexagonPlane base, FactoryFunction factory = nullptr) : HexagonPlane(base->hexSize(), factory), m_base(base) {}
	
	inline virtual HexagonIndexPair setHex(const sf::Vector3i& index, SPHexagon hexagon = nullptr) override {
		hexagon = HexagonPlane::setHex(index, hexagon).hexagon;
		return m_base->setHex(index, hexagon); 
	}

	inline float hexSize() const { return m_factory()->size(); }
	inline float hexWidth() const { return Hexagon::width(hexSize()); }
	inline float hexHeight() const { return Hexagon::height(hexSize()); }

	inline void update(float dt) { m_base->update(dt); }

protected:
	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(*m_base); }

private:
	SPHexagonPlane m_base;
};