#pragma once

#include "BigNumber.h"

#include "Hexagon.h"

#include <functional>
#include <SFML/Graphics.hpp>

class GameHex :
    public Hexagon
{
public:
	typedef std::shared_ptr<GameHex> SPGameHex;
	using SPAnimation = Animation::SPAnimation;

private:
	typedef std::function<void(const GameHex&, BigNumber)> YieldFunction;

public:
	GameHex(float size, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f));

	inline void setOnYield(YieldFunction onYield) { m_onYield = onYield; }

	void update(float dt) override;
private:
	void mouseEnter();
	void mouseExit();
	inline void mouseClick() { m_held = true; }
	inline void mouseRelease() { m_held = false; }

	/// Yield related

	YieldFunction m_onYield;

	// Once it reaches 1, it's reset and onYield is called
	float m_yieldProgress;
	// Progress added per second
	float m_yieldSpeed;
	// Multiplier to yield speed while the hex is being held down by the mouse
	float m_activeSpeedMultiplier;
	BigNumber m_yieldAmount;

	/// Event related

	// Whether or not the mouse is being held down on this hex
	bool m_held;
};