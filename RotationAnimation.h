#pragma once

#include "AnimationBase.h"

class RotationAnimation :
	public AnimationBase {
public:
	RotationAnimation(const std::vector<sf::Transformable*>& targets, float start, float end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);
	RotationAnimation(sf::Transformable* target, float start, float end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);

	virtual void activate(float duration, float offset = 0.0f) override;
private:
	void animate(float progress);

	std::vector<sf::Transformable*> m_targets;
	std::vector<float> m_offsets;

	const float m_start;
	const float m_end;
};