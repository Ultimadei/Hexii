#pragma once

#include <map>

#include "BezierCurvePresets.h"

class AnimationBase {
public:
	/// ** Private Structs & Definitions ** ///

	// Animate takes in 1 parameter: progress, as a float between 0 and 1 (should be returned by \
	// a bezier curve operation)
	typedef std::function<void(float)> AnimateFunction;
public:
	// The animate function will be called each frame if active
	// The bezier curve function is used to get the 'progress' of the animation at each timestep
	// Duration is how long (in seconds) the animation lasts. If 0, the animation will not be played to begin with
	AnimationBase(AnimateFunction animateFunction, BezierCurve bezierCurve, float duration = 0.0f);
	virtual inline ~AnimationBase() {}

	/// ** Public Member Functions ** ///

	/// Utility functions

	// The animation will start at offset seconds and will last for duration
	// If offset is negative, it is treated as a delay
	virtual void activate(float duration, float offset = 0.0f);
	// Terminates the animation
	void deactivate();

	// Toggles the animation on or off but preserves its current progress
	void setActive(bool active);

	// Returns true if the animation's duration > 0
	// If strict, only returns true if the animation is active and the duration > 0
	bool getActive(bool strict = true);

	// Returns the time elapsed in the current animation
	// If reset is true, 0.0f will be returned if the animation is complete
	float getElapsedTime(bool reset = false) const;

	/// Per-frame functions

	virtual void update(float dt);
private:
	/// ** Private Member Variables ** ///

	bool m_active;

	BezierCurve m_curve;

	sf::Clock m_clock;

	AnimateFunction m_animate;

	float m_duration;
	float m_elapsed;
};