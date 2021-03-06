#include "Hexagon.h"

#include "DisplayManager.h"
#include "ResourceManager.h"

Hexagon::Hexagon(float size, sf::Vector2f initialPosition) :
	ConvexShapeElement(6),
	m_size(size)
{
	for (unsigned int i = 0; i < 6; i++) {
		// Hexagon starts from a 30 degree rotation to make it flat top

		float angle = (30.0f + 60.0f * ((float)i + 1.0f)) * DEG_TO_RAD_CONSTANT;
		m_inner.setPoint(i, sf::Vector2f(std::sin(angle) * m_size, std::cos(angle) * m_size));
	}

	setPosition(initialPosition);
}

bool Hexagon::collidePoint(sf::Vector2f point) const {
	/// First check that point is inside the bounding box

	const sf::Vector2f pos = getPosition();

	sf::Transform rot;
	rot.rotate(getRotation());

	// Rotating the point about the hexagon's centre has the same result as rotating the Hexagon, but avoids Axis Alignment issues as it is a single point
	point -= pos;
	point = rot.transformPoint(point);

	sf::FloatRect bounds(-m_size, -(height() * 0.5f), width(), height());

	// Perform a collision check with the Hexagon bounds

	if (!bounds.contains(point)) { return false; }

	/// Now we leverage symmetry to detect by moving point into the bottom right quadrant (positive x, positive y)
	point.x = abs(point.x);
	point.y = abs(point.y);
	
	/*
	// middleRightCorner = (m_size, 0.0f)
	middleRightCorner is the 2nd point in the construction
	// bottomRightCorner = (m_size * 0.5f, halfHeight)
	bottomRightCorner is the 3rd point in the construction
	// edgeVectorNormal = (-halfHeight, -m_size * 0.5f)
	edgeVectorNormal is the normal to vector middleRightCorner >> bottomRightCorner \
	// pointVector = (pointTransformed.x - m_size, pointTransformed.y)
	pointVector is the vector middleRightCorner >> point
	*/
	
	// point is inside the hexagon if the dot product of pointVector and edgeVectorNormal >= 0
	return ((point.x - m_size) * height() * -0.5f) + (point.y * -m_size * 0.5f) >= 0.0f;
}