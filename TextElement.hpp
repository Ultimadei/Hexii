#pragma once

#include "Element.hpp"

// A wrapper around an sfml ConvexShape using the ElementBase interface
class TextElement :
	public Element {
public:
	typedef std::shared_ptr<TextElement> SPTextElement;

public:
	inline TextElement(SPFont font) : m_inner("", *font), m_font(font) {}
	inline virtual ~TextElement() {}

	/// SFML based functions

	inline void setFillColor(const sf::Color& fill) { m_inner.setFillColor(fill); }
	inline void setOutlineColor(const sf::Color& fill) { m_inner.setOutlineColor(fill); }
	inline void setOutlineThickness(float thickness) { m_inner.setOutlineThickness(thickness); }
	inline void setFont(SPFont font) { m_inner.setFont(*(m_font = font)); }
	inline void setCharacterSize(unsigned int size) { m_inner.setCharacterSize(size); }
	inline void setText(const std::string& text) { m_inner.setString(text); }

	inline sf::Color getFillColor() const { return m_inner.getFillColor(); }
	inline sf::Color getOutlineColor() const { return m_inner.getOutlineColor(); }
	inline float getOutlineThickness() const { return m_inner.getOutlineThickness(); }
	inline SPFont getFont() const { return m_font; }
	inline unsigned int getCharacterSize() { return m_inner.getCharacterSize(); }
	inline std::string getText() { return m_inner.getString(); }
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

	sf::Text m_inner;
	SPFont m_font;
};