#pragma once

#include "Animator.hpp"
#include "Element.hpp"
#include "SpriteElement.hpp"
#include "HexagonCluster.hpp"
#include "FloatAnimation.hpp"
#include "ColorAnimation.hpp"

class HexiiUpgradeGUI final :
	public Element {
private:
	using SPHexagon = HexagonCluster::SPHexagon;
	using SPSpriteElement = SpriteElement::SPSpriteElement;

public:
	HexiiUpgradeGUI();

	// Activate the overlay animation. Max 6 available upgrades (minimum 0)
	void activate(SPHexagon target, unsigned short availableUpgrades);
	void deactivate();

	inline float getProgress() const {
		float progress = std::max(m_animator["popoutTranslation"]->getProgress(), m_animator["popoutTranslationReverse"]->getProgress());
		printf("%f\n", progress);
		return progress;
	}

	inline void update(float dt) { 
		if (!active) return;
		m_animator.update(dt);
		m_nodes.update(dt);
	}

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void applyTranslation(const FloatAnimation& animation) const;
	void applyColor(const ColorAnimation& animation) const;
	
	SPHexagon generateUpgradeHex();

	Animator m_animator;
	HexagonPlane m_nodes;

	// TODO: Play around with this
	const float m_animationDuration = 0.75f;
};