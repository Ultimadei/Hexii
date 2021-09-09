#include "RotationAnimation.h"

RotationAnimation::RotationAnimation(const std::vector<sf::Transformable*>& targets, float start, float end, BezierCurve bezierCurve, float duration) :
	AnimationBase(std::bind(&RotationAnimation::animate, this, std::placeholders::_1), bezierCurve, duration),
	m_targets(targets),
	m_offsets(targets.size()),
	m_start(start),
	m_end(end)
{}

RotationAnimation::RotationAnimation(sf::Transformable* target, float start, float end, BezierCurve bezierCurve, float duration) :
	RotationAnimation(std::vector<sf::Transformable*>(1, target), start, end, bezierCurve, duration)
{}

void RotationAnimation::activate(float duration, float offset) {
	// On activation, as well as default behavior, reset the stored offset of target

	AnimationBase::activate(duration, offset);

	for (unsigned int i = 0; i < m_targets.size(); i++) {
		m_offsets[i] = m_targets[i]->getRotation();
	}
}

void RotationAnimation::animate(float progress) {
	for (unsigned int i = 0; i < m_targets.size(); i++) {
		m_targets[i]->setRotation(m_offsets[i] + (m_start * (1.0f - progress)) + (m_end * progress));
	}
}