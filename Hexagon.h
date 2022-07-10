#pragma once

#include "Trig.h"

#include "ElementParent.h"
#include "AnimationParent.h"
#include "ConvexShapeElement.h"

class Hexagon :
	public ElementParent,
	public AnimationParent,
	public ConvexShapeElement {
public:
	// Construct with size and optionally with a starting position and/or hexii data
	Hexagon(float size, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f));
	// Copy constructor
	Hexagon(const Hexagon& right);
	inline ~Hexagon() {}

	/// ** Public member functions

	// Checks if there is a collision with `point`
	bool collidePoint(sf::Vector2f point) const;	

	/// Utility 

	inline static float width(float size) { return size * 2.0f; }
	inline static float height(float size) { return size * CONSTANT_SQRT3; }

	inline float size() const { return m_size; }
	inline float width() const { return m_size * 2.0f; }
	inline float height() const { return m_size * CONSTANT_SQRT3; }

	// Preserves own size and children. Everything else is copied from right
	void operator=(const Hexagon& right);

	/// Callback

	inline virtual void onMouseEnter() {}
	inline virtual void onMouseExit() {}
	inline virtual void onMouseClick() {}
	inline virtual void onMouseRelease() {}

	virtual void update(float dt);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	// "size" refers to the side length
	const float m_size;
};