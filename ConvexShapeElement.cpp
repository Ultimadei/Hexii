#include "ConvexShapeElement.h"

ConvexShapeElement::ConvexShapeElement(float points) :
	m_inner(points)
{}

ConvexShapeElement::ConvexShapeElement(const ConvexShapeElement& right) :
	m_inner(right.m_inner)
{}

ConvexShapeElement::ConvexShapeElement(ConvexShapeElement&& right) noexcept :
	m_inner(right.m_inner) {}

void ConvexShapeElement::setFillColor(const sf::Color& fill) {
	m_inner.setFillColor(fill);
}

void ConvexShapeElement::setOutlineColor(const sf::Color& fill) {
	m_inner.setOutlineColor(fill);
}

void ConvexShapeElement::setOutlineThickness(float thickness) {
	m_inner.setOutlineThickness(thickness);
}

void ConvexShapeElement::setTexture(const sf::Texture* texture, bool resetRect) {
	m_inner.setTexture(texture, resetRect);
}

void ConvexShapeElement::setTextureRect(const sf::IntRect& rect) {
	m_inner.setTextureRect(rect);
}

sf::Color ConvexShapeElement::getFillColor() const {
	return m_inner.getFillColor();
}

sf::Color ConvexShapeElement::getOutlineColor() const {
	return m_inner.getOutlineColor();
}

float ConvexShapeElement::getOutlineThickness() const {
	return m_inner.getOutlineThickness();
}

const sf::Texture* ConvexShapeElement::getTexture() const {
	return m_inner.getTexture();
}

sf::IntRect ConvexShapeElement::getTextureRect() const {
	return m_inner.getTextureRect();
}

void ConvexShapeElement::operator=(const ConvexShapeElement& right) {
	ElementBase::operator=(right);
}

void ConvexShapeElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.combine(getTransform());

	if (m_shader != nullptr) states.shader = m_shader;

	target.draw(m_inner, states);
}