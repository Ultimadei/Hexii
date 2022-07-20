#pragma once

#include "Element.hpp"

// A wrapper around an sfml ConvexShape using the ElementBase interface
class SpriteElement :
	public Element {
public:
	typedef std::shared_ptr<SpriteElement> SPSpriteElement;

public:
	inline SpriteElement() : m_texture(nullptr) {}
	inline SpriteElement(SPTexture texture) : m_inner(*texture), m_texture(texture) {}

	inline virtual ~SpriteElement() {}

	/// SFML based functions

	inline void setColor(const sf::Color& color) { m_inner.setColor(color); }
	inline void setTexture(SPTexture texture, bool resetRect = true) {
		// Replace the raw pointer first so that it is never dangling
		m_inner.setTexture(*texture, resetRect);
		// This may cause the current contents of `m_texture` to be deleted
		m_texture = texture;
	}
	inline void setTextureRect(const sf::IntRect& rect) { m_inner.setTextureRect(rect); }

	inline sf::Color getColor() const { return m_inner.getColor(); }
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

	sf::Sprite m_inner;
	SPTexture m_texture;
};