#include "AnimationBase.h"

AnimationBase::AnimationBase(AnimateFunction animateFunction, BezierCurve bezierCurve, float duration) :
	m_active(true),
	m_curve(bezierCurve),
	m_animate(animateFunction),
	m_duration(duration),
	m_elapsed(0.0f)
{
	if(duration > 0.0f) { activate(duration); }	
	else { deactivate(); }
}

void AnimationBase::activate(float duration, float offset) {
	setActive(true);
	m_duration = duration;
	m_elapsed = offset;
}

void AnimationBase::deactivate() {
	setActive(false);
	m_duration = 0.0f;
	m_elapsed = 0.0f;
}

void AnimationBase::setActive(bool active) {
	m_active = active;
	if (m_elapsed == m_duration) { m_elapsed = 0.0f; } // If the animation is complete, restart it
	m_clock.restart();
}

bool AnimationBase::getActive(bool strict) {
	if (strict) return m_active;
	else if (m_duration > 0.0f) return true;
	return false;
}

float AnimationBase::getElapsedTime(bool reset) const {
	if (reset && m_elapsed == m_duration) { return 0.0f; }
	return m_elapsed;
}

void AnimationBase::update(float dt) {
	if (m_active && m_duration > 0.0f) {
		m_elapsed += m_clock.restart().asSeconds();

		float progress = m_elapsed / m_duration;

		if (progress < 0.0f) progress = 0.0f;
		if (progress > 1.0f) {
			m_elapsed = m_duration;
			m_active = false;
			// Due to rounding, the intended destination can be missed slightly
			// To fix that, animate(1)
			m_animate(1.0f);
		}
		else {
			m_animate(m_curve(progress));
		}
	}
}