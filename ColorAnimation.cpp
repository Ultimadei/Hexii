#include "ColorAnimation.h"

ColorAnimation::ColorAnimation(const std::vector<ElementBase*>& targets, const sf::Color& start, const sf::Color& end, BezierCurve bezierCurve, float duration) :
	AnimationBase(std::bind(&ColorAnimation::animate, this, std::placeholders::_1), bezierCurve, duration),
	m_targets(targets),
	m_start(start),
	m_end(end)
{}

ColorAnimation::ColorAnimation(ElementBase* target, const sf::Color& start, const sf::Color& end, BezierCurve bezierCurve, float duration) :
	ColorAnimation(std::vector<ElementBase*>(1, target), start, end, bezierCurve, duration)
{}

void ColorAnimation::animate(float progress) {
	if (progress >= 1.0f) { progress = 1.0f; }

	sf::Color adjustedStart = m_start;
	sf::Color adjustedEnd = m_end;

	adjustedStart.r = (char)std::floor((float)adjustedStart.r * (1.0f - progress));
	adjustedStart.g = (char)std::floor((float)adjustedStart.g * (1.0f - progress));
	adjustedStart.b = (char)std::floor((float)adjustedStart.b * (1.0f - progress));
	adjustedStart.a = (char)std::floor((float)adjustedStart.a * (1.0f - progress));

	adjustedEnd.r = (char)std::floor((float)adjustedEnd.r * progress);
	adjustedEnd.g = (char)std::floor((float)adjustedEnd.g * progress);
	adjustedEnd.b = (char)std::floor((float)adjustedEnd.b * progress);
	adjustedEnd.a = (char)std::floor((float)adjustedEnd.a * progress);

	sf::Color output = adjustedStart + adjustedEnd;

	for (unsigned int i = 0; i < m_targets.size(); i++) {
		m_targets[i]->setFillColor(output);
	}
}