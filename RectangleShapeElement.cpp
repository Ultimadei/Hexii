#include "RectangleShapeElement.h"

RectangleShapeElement::RectangleShapeElement(const sf::Vector2f& size) :
	m_inner(size)
{}

RectangleShapeElement::RectangleShapeElement(const RectangleShapeElement& right) :
	m_inner(right.m_inner)
{}

RectangleShapeElement::RectangleShapeElement(RectangleShapeElement&& right) noexcept :
	m_inner(right.m_inner) {}

void RectangleShapeElement::setFillColor(const sf::Color& fill) {
	m_inner.setFillColor(fill);
}

void RectangleShapeElement::setOutlineColor(const sf::Color& fill) {
	m_inner.setOutlineColor(fill);
}

void RectangleShapeElement::setOutlineThickness(float thickness) {
	m_inner.setOutlineThickness(thickness);
}

void RectangleShapeElement::setTexture(const sf::Texture* texture, bool resetRect) {
	m_inner.setTexture(texture, resetRect);
}

void RectangleShapeElement::setTextureRect(const sf::IntRect& rect) {
	m_inner.setTextureRect(rect);
}

void RectangleShapeElement::setSize(const sf::Vector2f& size) {
	m_inner.setSize(size);
}

sf::Color RectangleShapeElement::getFillColor() const {
	return m_inner.getFillColor();
}

sf::Color RectangleShapeElement::getOutlineColor() const {
	return m_inner.getOutlineColor();
}

float RectangleShapeElement::getOutlineThickness() const {
	return m_inner.getOutlineThickness();
}

const sf::Texture* RectangleShapeElement::getTexture() const {
	return m_inner.getTexture();
}

sf::IntRect RectangleShapeElement::getTextureRect() const {
	return m_inner.getTextureRect();
}

sf::Vector2f RectangleShapeElement::getSize() const {
	return m_inner.getSize();
}

void RectangleShapeElement::operator=(const RectangleShapeElement& right) {
	ElementBase::operator=(right);
}

void RectangleShapeElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.combine(getTransform());

	target.draw(m_inner, states);
}