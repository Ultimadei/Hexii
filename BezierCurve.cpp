#include "BezierCurve.h"

BezierCurve::BezierCurve(const std::vector<sf::Vector2f>& points) :
	m_pointCount(points.size()),
	m_points(points)
{}

float BezierCurve::interpolate(float t) const {
	/// Handle the special cases

	// 0 and 1 are not curves
	if (m_pointCount == 0) return t;
	if (m_pointCount == 1) return m_points[0].y;

	// 2 is a straight line
	if (m_pointCount == 2) {
		return linearInterpolate(m_points[0], m_points[1], t).y;
	}

	// Stores the previous iteration
	std::vector<sf::Vector2f> previous = m_points;

	// The recursive definition is used for point counts > 2
	for (unsigned int i = m_pointCount - 2; i > 0; i--) {
		std::vector<sf::Vector2f> intermediaries;

		for (unsigned int j = 0; j < previous.size() - 1; j++) {
			intermediaries.push_back(
				linearInterpolate(previous[j], previous[j + 1], t)
			);
		}
		
		previous.clear();
		previous = intermediaries;
	}

	return linearInterpolate(previous[0], previous[1], t).y;
}

float BezierCurve::operator()(float t) const {
	return interpolate(t);
}

sf::Vector2f BezierCurve::linearInterpolate(const sf::Vector2f& a, const sf::Vector2f& b, float t) const {
	return sf::Vector2f(
		(a.x * (1.0f - t)) + (b.x * t),
		(a.y * (1.0f - t)) + (b.y * t)
	);
}
