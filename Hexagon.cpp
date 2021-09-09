#include "Hexagon.h"

#include "Trig.h"

Hexagon::Hexagon(float size, sf::Vector2f initialPosition, HexiiData* initialData) :
	ConvexShapeElement(6),
	m_size(size),
	m_data(initialData)
{
	for (unsigned int i = 0; i < 6; i++) {
		// Hexagon starts from a 30 degree rotation for aestethic reasons

		float angle = degreesToRadians(30.0f + 60.0f * ((float)i + 1.0f));
		m_inner.setPoint(i, sf::Vector2f(std::sin(angle) * m_size, std::cos(angle) * m_size));
	}

	setPosition(initialPosition);
}

Hexagon::Hexagon(float size, float x, float y, HexiiData* initialData) :
	Hexagon(size, sf::Vector2f(x, y), initialData) 
{}

Hexagon::Hexagon(const Hexagon& right) :
	ConvexShapeElement(right),
	m_size(right.m_size),
	m_data(nullptr)
{
	operator=(right);
}

Hexagon::~Hexagon() {
	if (m_data != nullptr) {
		delete m_data;
	}
}

bool Hexagon::collidePoint(sf::Vector2f point) const {
	// First check point is inside the bounding box

	const sf::Vector2f hexagonPosition = getPosition();

	sf::Transform rotationTransform;
	rotationTransform.rotate(getRotation());

	sf::FloatRect bounds(-m_size, -(height() * 0.5f), width(), height());
	bounds = rotationTransform.transformRect(bounds);
	bounds.left += hexagonPosition.x;
	bounds.top += hexagonPosition.y;

	/*if (point.x > centerPosition.x + m_size ||
		point.x < centerPosition.x - m_size ||
		point.y > centerPosition.y + halfHeight ||
		point.y < centerPosition.y - halfHeight
	) {
		// Point did not collide with the bounding box
		return false;
	}*/
	if (!bounds.contains(point)) { return false; }

	// Transform point into hexagon's local coordinates
	point -= hexagonPosition;
	point = rotationTransform.transformPoint(point);

	point.x = std::abs(point.x);
	point.y = std::abs(point.y);
	
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

const HexiiData* Hexagon::getData() const {
	return m_data;
}

HexiiData* Hexagon::accessData() {
	if (m_data == nullptr) {
		m_data = new HexiiData;
	}
	return m_data;
}

void Hexagon::center(const sf::FloatRect& target, bool horizontal, bool vertical) {
	setPosition((target.left + target.width) * 0.5f, (target.top + target.height) * 0.5f);
}

float Hexagon::size() const {
	return m_size;
}

float Hexagon::width() const {
	return 2.0f * m_size;
}

float Hexagon::height() const {
	return m_size * CONSTANT_SQRT3;
}

void Hexagon::operator=(const Hexagon& copy) {
	ConvexShapeElement::operator=(copy);
	sf::Transformable::operator=(copy);
}

void Hexagon::update(float dt) {
	updateAnimations(dt);
}

void Hexagon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Draw this
	ConvexShapeElement::draw(target, states);

	states.transform.combine(getTransform());

	// Draw children
	drawChildren(target, states);
}