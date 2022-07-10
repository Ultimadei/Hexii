#include "AbstractElementBase.h"

AbstractElementBase::AbstractElementBase() :
	m_shader(nullptr), m_active(true)
{}

void AbstractElementBase::setShader(sf::Shader* shader) {
	m_shader = shader;
}

void AbstractElementBase::setActive(bool active) {
	m_active = active;
}

bool AbstractElementBase::getActive() const {
	return m_active;
}

void AbstractElementBase::operator=(const AbstractElementBase& right) {
	setFillColor(right.getFillColor());
	setOutlineColor(right.getOutlineColor());
	setOutlineThickness(right.getOutlineThickness());
	setTexture(right.getTexture());
	setTextureRect(right.getTextureRect());
	setShader(right.m_shader);
	setActive(right.getActive());
}