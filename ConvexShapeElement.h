#pragma once

#include "ElementBase.h"

// A wrapper around an sfml ConvexShape using the ElementBase interface
class ConvexShapeElement :
	public ElementBase {
public:
	ConvexShapeElement(float points);
	ConvexShapeElement(const ConvexShapeElement& right); // Copy constructor
	ConvexShapeElement(ConvexShapeElement&& right) noexcept; // Move constructor

	inline virtual ~ConvexShapeElement() {}

	/// SFML based functions

	virtual void setFillColor(const sf::Color& fill) override;
	virtual void setOutlineColor(const sf::Color& fill) override;
	virtual void setOutlineThickness(float thickness) override;
	virtual void setTexture(const sf::Texture* texture, bool resetRect = true) override;
	virtual void setTextureRect(const sf::IntRect& rect) override;

	sf::Color getFillColor() const override;
	sf::Color getOutlineColor() const override;
	float getOutlineThickness() const override;
	const sf::Texture* getTexture() const override;
	sf::IntRect getTextureRect() const override;

	void operator=(const ConvexShapeElement& right);
protected:
	sf::ConvexShape m_inner;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};