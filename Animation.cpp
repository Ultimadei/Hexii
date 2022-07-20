#include "Animation.h"
#include "DisplayManager.h"

Animation::Animation(BezierCurve curve, float duration, bool begin) :
	m_active(begin && duration > 0.0f),
	m_curve(curve),
	m_duration(duration),
	m_elapsed(0.0f)
{
	if (m_active) play();
}

void Animation::play(float offset) {
	m_active = true;
	// Restart if the animation had finished
	if (m_elapsed == m_duration) m_elapsed = 0.0f;
	m_elapsed += offset;
}

void Animation::stop() {
	m_elapsed = 0.0f;
	pause();
}

void Animation::restart(float from) {
	m_elapsed = from;
	play();
}

void Animation::restart(float duration, float from) {
	m_duration = duration;
	restart(from);
}

void Animation::update(float dt) {
	if (!m_active || m_duration == 0.0f) return;

	m_elapsed += dt / DisplayManager::getTargetFPS();

	float progress = getProgress();

	if (progress < 0.0f) progress = 0.0f;
	else if (progress > 1.0f) {
		m_elapsed = m_duration;
		m_active = false;
		// Due to rounding, animate(1) may not be called otherwise
		animate(1.0f);
	}
	else animate(m_curve(progress));
}