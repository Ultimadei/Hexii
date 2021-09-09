#pragma once

#include "AnimationBase.h"
#include "ElementBase.h"

class ColorAnimation :
	public AnimationBase {
public:
	ColorAnimation(const std::vector<ElementBase*>& targets, const sf::Color& start, const sf::Color& end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);
	ColorAnimation(ElementBase* target, const sf::Color& start, const sf::Color& end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);
private:
	void animate(float progress);

	std::vector<ElementBase*> m_targets;

	const sf::Color m_start;
	const sf::Color m_end;
};