#pragma once

#include "Element.hpp"

// A wrapper around an sfml ConvexShape using the ElementBase interface
class ConvexShapeElement :
	public Element {
public:
	inline ConvexShapeElement(float points) : m_inner(points), m_texture(nullptr) {}

	inline virtual ~ConvexShapeElement() {}

	/// SFML based functions

	inline void setFillColor(const sf::Color& fill) { m_inner.setFillColor(fill); }
	inline void setOutlineColor(const sf::Color& fill) { m_inner.setOutlineColor(fill); }
	inline void setOutlineThickness(float thickness) { m_inner.setOutlineThickness(thickness); }
	inline void setTexture(SPTexture texture, bool resetRect = true) { m_inner.setTexture((m_texture = texture).get(), resetRect); }
	inline void setTextureRect(const sf::IntRect& rect) { m_inner.setTextureRect(rect); }

	inline sf::Color getFillColor() const { return m_inner.getFillColor(); }
	inline sf::Color getOutlineColor() const { return m_inner.getOutlineColor(); }
	inline float getOutlineThickness() const { return m_inner.getOutlineThickness(); }
	inline const SPTexture getTexture() const { return m_texture; }
	inline sf::IntRect getTextureRect() const { return m_inner.getTextureRect(); }
	inline sf::FloatRect getLocalBounds() { return m_inner.getLocalBounds(); }
	inline sf::FloatRect getGlobalBounds() { return m_inner.getGlobalBounds(); }

protected:
	inline virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		if (!active) return;

		states.shader = m_shader.get();
		states.transform.combine(getTransform());
		target.draw(m_inner, states);

		states.shader = nullptr;
		drawSubElements(target, states);
	}

	sf::ConvexShape m_inner;
	SPTexture m_texture;	
};