#pragma once

#include "Animation.h"
#include "Element.hpp"

// ColorAnimation just generates the desired color each frame. It's up to the animation's owner to decide what to do with the color
class ColorAnimation :
	public Animation {
private:
	typedef std::function<void(const ColorAnimation&)> ApplyFunction;

public:
	inline ColorAnimation(ApplyFunction apply, const sf::Color& start, const sf::Color& end, BezierCurve curve = BezierCurves::linear, float duration = 0.0f, bool begin = false)
		: Animation(curve, duration, begin), m_apply(apply), m_start(start), m_end(end) {}

	inline sf::Color getOutput() const { return m_output; }
protected:
	inline virtual void animate(float progress) override {
		if (progress >= 1.0f) progress = 1.0f;

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

		m_output = adjustedStart + adjustedEnd;

		m_apply(*this);
	}

private:
	const sf::Color m_start;
	const sf::Color m_end;

	sf::Color m_output;

	ApplyFunction m_apply;
};