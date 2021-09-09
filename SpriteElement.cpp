#include "SpriteElement.h"

SpriteElement::SpriteElement(const sf::Texture* texture) :
	m_inner(*texture)
{}

SpriteElement::SpriteElement(const SpriteElement& right) :
	m_inner(right.m_inner)
{}

SpriteElement::SpriteElement(SpriteElement&& right) noexcept :
	m_inner(right.m_inner)
{}

void SpriteElement::setFillColor(const sf::Color& fill) {
	m_inner.setColor(fill);
}

void SpriteElement::setTexture(const sf::Texture* texture, bool resetRect) {
	m_inner.setTexture(*texture, resetRect);
}

void SpriteElement::setTextureRect(const sf::IntRect& rect) {
	m_inner.setTextureRect(rect);
}

sf::Color SpriteElement::getFillColor() const {
	return m_inner.getColor();
}

const sf::Texture* SpriteElement::getTexture() const {
	return m_inner.getTexture();
}

sf::IntRect SpriteElement::getTextureRect() const {
	return m_inner.getTextureRect();
}

sf::Color SpriteElement::getOutlineColor() const {
	return sf::Color::Black;
}

float SpriteElement::getOutlineThickness() const {
	return 0.0f;
}

void SpriteElement::operator=(const SpriteElement& right) {
	ElementBase::operator=(right);
}

void SpriteElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.combine(getTransform());

	target.draw(m_inner, states);
}