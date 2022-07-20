#pragma once

#include "Animation.h"

class FloatAnimation :
	public Animation {
private:
	typedef std::function<void(const FloatAnimation&)> ApplyFunction;

public:
	inline FloatAnimation(ApplyFunction apply, float start, float end, BezierCurve curve = BezierCurves::linear, float duration = 0.0f, bool begin = false)
		: Animation(curve, duration, begin), m_apply(apply), m_offset(0.0f), m_output(0.0f), m_start(start), m_end(end) {}

	inline void setOffset(float offset) { m_offset = offset; }

	inline float getOutput() const { return m_output; }

protected:
	inline virtual void animate(float progress) override { 
		m_output = m_offset + (m_start * (1.0f - progress)) + (m_end * progress); 
	
		m_apply(*this);
	}

private:
	float m_offset;

	const float m_start;
	const float m_end;

	float m_output;

	ApplyFunction m_apply;
};