#pragma once

#include "Animation.h"

class Vector2fAnimation :
	public Animation {
private:
	typedef std::function<void(const Vector2fAnimation&)> ApplyFunction;
public:
	inline Vector2fAnimation(ApplyFunction apply, const sf::Vector2f& start, const sf::Vector2f& end, BezierCurve curve = BezierCurves::linear, float duration = 0.0f, bool begin = false)
		: Animation(curve, duration, begin), m_apply(apply), m_start(start), m_end(end) {}

	inline void setOffset(const sf::Vector2f offset) { m_offset = offset; }

	inline sf::Vector2f getOutput() const { return m_output; }
	
protected:
	inline virtual void animate(float progress) override {
		m_output = m_offset + (m_start * (1.0f - progress)) + (m_end * progress); 
		
		m_apply(*this);
	}

private:
	sf::Vector2f m_offset;

	const sf::Vector2f m_start;
	const sf::Vector2f m_end;

	sf::Vector2f m_output;

	ApplyFunction m_apply;
};