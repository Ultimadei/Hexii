#pragma once

#include "Trig.hpp"

#include "Animator.hpp"
#include "ConvexShapeElement.hpp"

class Hexagon :
	public ConvexShapeElement {
public:
	typedef std::shared_ptr<Hexagon> SPHexagon;

protected:
	typedef std::function<void()> InputCallbackFunction;

public:
	// Construct with size and optionally a starting position
	Hexagon(float size, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f));

	inline virtual ~Hexagon() {}

	// Checks if there is a collision with `point`
	bool collidePoint(sf::Vector2f point) const;	

	/// Utility 

	inline static float width(float size) { return size * 2.0f; }
	inline static float height(float size) { return size * SQRT3; }

	inline float size() const { return m_size; }
	inline float width() const { return m_size * 2.0f; }
	inline float height() const { return m_size * SQRT3; }

	/// Callbacks

	inline void onMouseEnter() { if (m_onMouseEnter) m_onMouseEnter(); }
	inline void onMouseExit() { if (m_onMouseExit) m_onMouseExit(); }
	inline void onMouseClick() { if (m_onMouseClick) m_onMouseClick(); }
	inline void onMouseRelease() { if (m_onMouseRelease) m_onMouseRelease(); }

	inline virtual void update(float dt) { animator.update(dt); }

	// Can be accessed publicly as the gateway to animations
	Animator animator;

protected:
	InputCallbackFunction m_onMouseEnter;
	InputCallbackFunction m_onMouseExit;
	InputCallbackFunction m_onMouseClick;
	InputCallbackFunction m_onMouseRelease;

private:
	// "size" refers to the side length
	const float m_size;
};