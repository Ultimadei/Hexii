#pragma once

#include "ElementBase.h"

// A wrapper around an sfml RectangleShape using the ElementBase interface
class RectangleShapeElement :
	public ElementBase {
public:
	RectangleShapeElement(const sf::Vector2f& size);
	RectangleShapeElement(const RectangleShapeElement& right); // Copy constructor
	RectangleShapeElement(RectangleShapeElement&& right) noexcept; // Move constructor

	inline virtual ~RectangleShapeElement() {}

	/// SFML based functions

	virtual void setFillColor(const sf::Color& fill) override;
	virtual void setOutlineColor(const sf::Color& fill) override;
	virtual void setOutlineThickness(float thickness) override;
	virtual void setTexture(const sf::Texture* texture, bool resetRect = true) override;
	virtual void setTextureRect(const sf::IntRect& rect) override;
	virtual void setSize(const sf::Vector2f& size);

	sf::Color getFillColor() const override;
	sf::Color getOutlineColor() const override;
	float getOutlineThickness() const override;
	const sf::Texture* getTexture() const override;
	sf::IntRect getTextureRect() const override;
	sf::Vector2f getSize() const;

	void operator=(const RectangleShapeElement& right);
protected:
	sf::RectangleShape m_inner;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};