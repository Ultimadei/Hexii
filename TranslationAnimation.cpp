#include "TranslationAnimation.h"

TranslationAnimation::TranslationAnimation(const std::vector<sf::Transformable*>& targets, sf::Vector2f start, sf::Vector2f end, BezierCurve bezierCurve, float duration) : 
	AnimationBase(std::bind(&TranslationAnimation::animate, this, std::placeholders::_1), bezierCurve, duration),
	m_targets(targets),
	m_offsets(targets.size()),
	m_start(start),
	m_end(end)
{}

TranslationAnimation::TranslationAnimation(sf::Transformable* target, sf::Vector2f start, sf::Vector2f end, BezierCurve bezierCurve, float duration) :
	TranslationAnimation(std::vector<sf::Transformable*>(1, target), start, end, bezierCurve, duration)
{}

void TranslationAnimation::activate(float duration, float offset) {
	// On activation, as well as default behavior, reset the stored offset of target

	AnimationBase::activate(duration, offset);

	for (unsigned int i = 0; i < m_targets.size(); i++) {
		m_offsets[i] = m_targets[i]->getPosition();
	}
}

void TranslationAnimation::animate(float progress) {
	for (unsigned int i = 0; i < m_targets.size(); i++) {
		m_targets[i]->setPosition(m_offsets[i] + (m_start * (1.0f - progress)) + (m_end * progress));
	}
}