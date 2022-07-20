#pragma once

#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>

// TODO: Verify this works as intended

class BezierCurve {
public:
	typedef std::function<float(float)> BezierCurveFunction;
public:
	BezierCurve(const std::vector<sf::Vector2f>& points);

	// t should be a value between 0 and 1 which is used to determine the output
	float interpolate(float t) const;

	float operator()(float t) const;
private:
	sf::Vector2f linearInterpolate(const sf::Vector2f& a, const sf::Vector2f& b, float t) const;

	// 255 maximum should be more than enough!
	const unsigned short m_pointCount;
	const std::vector<sf::Vector2f> m_points;	
};