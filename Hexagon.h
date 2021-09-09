#pragma once

#include "ElementParent.h"
#include "AnimationParent.h"
#include "ConvexShapeElement.h"

#include "HexiiData.h"

class Hexagon :
	public ElementParent,
	public AnimationParent,
	public ConvexShapeElement {
public:
	// Construct with size and optionally with a starting position and/or hexii data
	Hexagon(float size, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f), HexiiData* initialData = nullptr);
	// Construct from primatives
	Hexagon(float size, float x, float y, HexiiData* initialData = nullptr);
	// Copy constructor
	Hexagon(const Hexagon& right);
	~Hexagon();

	// Checks if there is a collision with point
	bool collidePoint(sf::Vector2f point) const;

	const HexiiData* getData() const;
	// Returns the data object
	// Note: If none exists, this function will create it first
	HexiiData* accessData();

	void center(const sf::FloatRect& target, bool horizontal = true, bool vertical = true);

	float size() const;
	float width() const;
	float height() const;

	// Preserves own size, data and all children. Everything else is copied from right
	void operator=(const Hexagon& right);

	virtual void update(float dt);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const float m_size;

	HexiiData* m_data;
};