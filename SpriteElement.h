#pragma once

#include "ElementBase.h"

// A wrapper around an sfml ConvexShape using the ElementBase interface
class SpriteElement :
	public ElementBase {
public:
	inline SpriteElement() {}
	SpriteElement(const sf::Texture* texture);
	SpriteElement(const SpriteElement& right); // Copy constructor
	SpriteElement(SpriteElement&& right) noexcept; // Move constructor

	inline virtual ~SpriteElement() {}

	/// SFML based functions

	virtual void setFillColor(const sf::Color& fill) override;
	virtual void setTexture(const sf::Texture* texture, bool resetRect = true) override;
	virtual void setTextureRect(const sf::IntRect& rect) override;

	sf::Color getFillColor() const override;
	const sf::Texture* getTexture() const override;
	sf::IntRect getTextureRect() const override;

	// No available implementation for Sprites
	virtual inline void setOutlineColor(const sf::Color& fill) override {}
	// No available implementation for Sprites
	virtual inline void setOutlineThickness(float thickness) override {}

	// No available implementation for Sprites. Returns an opaque black
	virtual sf::Color getOutlineColor() const override;
	// No available implementation for Sprites. Returns 0.0f
	virtual float getOutlineThickness() const override;

	void operator=(const SpriteElement& right);
protected:
	sf::Sprite m_inner;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};