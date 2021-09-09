#pragma once

#include <SFML/Graphics.hpp>

#include "Hexagon.h"

class HexiiUpgradeOverlay :
	public sf::Drawable,
	public sf::Transformable {
public:
	HexiiUpgradeOverlay(const sf::Vector2f& basePosition = sf::Vector2f(0.0f, 0.0f));

	// Activate the overlay animation. Max 6 available upgrades (minimum 0)
	void activate(const Hexagon* target, short availableUpgrades);
	void deactivate();

	// Sets the 
	void setBasePosition(const sf::Vector2f& basePosition);

	bool getActive() const;
	// Returns the progress of the animation (or 1.0f if active and finished, 0.0f if inactive and finished)
	float getProgress() const;

	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	bool m_active;

	sf::Vector2f m_basePosition;
	Hexagon m_upgradeNodes[6];
};