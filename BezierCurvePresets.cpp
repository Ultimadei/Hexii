#include "BezierCurvePresets.h"

namespace BezierCurves {
	BezierCurve easeInOut({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(0.45f, 0.0f),
		sf::Vector2f(0.55f, 1.0f),
		sf::Vector2f(1.0f, 1.0f)
	});

	BezierCurve easeInOutBidirectional({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(0.225f, 0.0f),
		sf::Vector2f(0.275f, 2.0f),
		sf::Vector2f(0.5f, 2.0f),
		sf::Vector2f(0.725f, 2.0f),
		sf::Vector2f(0.775f, 0.0f),
		sf::Vector2f(0.0f, 0.0f),
	});

	BezierCurve linear({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(1.0f, 1.0f)
	});

	BezierCurve linearBidirectional({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(0.5f, 2.0f),
		sf::Vector2f(0.0f, 0.0f)
	});

	BezierCurve elasticIn({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(0.7f, -0.55f),
		sf::Vector2f(0.27f, 1.00f),
		sf::Vector2f(1.0f, 1.0f)
	});

	BezierCurve elasticOut({
		sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f(0.7f, 0.0f),
		sf::Vector2f(0.27f, 1.55f),
		sf::Vector2f(1.0f, 1.0f)
	});
}