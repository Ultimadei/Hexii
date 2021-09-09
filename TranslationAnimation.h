#pragma once

#include "AnimationBase.h"

class TranslationAnimation :
	public AnimationBase {
public:
	TranslationAnimation(const std::vector<sf::Transformable*>& targets, sf::Vector2f start, sf::Vector2f end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);
	TranslationAnimation(sf::Transformable* target, sf::Vector2f start, sf::Vector2f end, BezierCurve bezierCurve = BezierCurvePresets::linear, float duration = 0.0f);

	virtual void activate(float duration, float offset = 0.0f) override;
private:
	const std::vector<sf::Transformable*> m_targets;
	std::vector<sf::Vector2f> m_offsets;
	const sf::Vector2f m_start;
	const sf::Vector2f m_end;

	void animate(float progress);
};