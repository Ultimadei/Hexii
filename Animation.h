#pragma once

#include <map>
#include <functional>

#include "BezierCurvePresets.h"

class Animation {
public:
	typedef std::shared_ptr<Animation> SPAnimation;

public:
	// The bezier curve function is used to get the 'progress' of the animation at each timestep
	// `duration` is how long (in seconds) the animation lasts. If greater than 0 and `begin` is true then the animation will start immediately
	Animation(BezierCurve curve, float duration = 0.0f, bool begin = false);
	virtual inline ~Animation() {}

	// Continues the animation with an optional offset (negative time elapsed acts as a delay)
	void play(float offset = 0.0f);
	inline void pause() { m_active = false; }

	// Restarts and pauses
	void stop();

	// Restarts the animation with the same duration as last time starting `from` seconds in
	void restart(float from = 0.0f);
	// Restarts the animation with a new duration starting `from` seconds in
	void restart(float duration, float from);

	// Returns the fraction of the animation that's elapsed. Note: returns 1.0 if duration is 0
	inline float getProgress() const { return m_duration == 0.0f ? 1.0f : m_elapsed / m_duration; }

	void update(float dt);

protected:
	// Should set any state needed for the objects that use this animation
	virtual void animate(float progress) = 0;

private:
	bool m_active;

	BezierCurve m_curve;

	float m_duration;
	float m_elapsed;
};