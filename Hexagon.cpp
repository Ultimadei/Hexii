#include "Hexagon.h"

#include "Trig.h"

#include "DisplayManager.h"
#include "ResourceManager.h"

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

	// Rotating the point about the hexgaon's center has the same result as rotating the Hexagon, but avoids Axis Alignment issues as it is a single point
	point -= hexagonPosition;
	point = rotationTransform.transformPoint(point);

	// sf::FloatRect bounds(hexagonPosition.x - m_size, hexagonPosition.y - (height() * 0.5f), width(), height());
	sf::FloatRect bounds(-m_size, -(height() * 0.5f), width(), height());

	// Perform a collision check with the Hexagon bounds

	/*if (point.x > centerPosition.x + m_size ||
		point.x < centerPosition.x - m_size ||
		point.y > centerPosition.y + halfHeight ||
		point.y < centerPosition.y - halfHeight
	) {
		// Point did not collide with the bounding box
		return false;
	}*/
	if (!bounds.contains(point)) { return false; }

	// Move point into the bottom right quadrant (positive x, positive y)
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
	if (m_shader != nullptr) {
		// TODO: Get rid of bad tmp code
		static sf::Clock timer;
		m_shader->setUniform("time", timer.getElapsedTime().asSeconds());
		auto t = ResourceManager::getTexture("hexProgressBar");
		setTexture(t);
		m_shader->setUniform("progressBar", *t);
		auto a = sf::Vector2f(DisplayManager::worldToScreen(getPosition()));
		// printf("%d %d\n", (int)a.x, (int)a.y);
		m_shader->setUniform("center", a);
	}
}

void Hexagon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Draw this
	ConvexShapeElement::draw(target, states);

	states.transform.combine(getTransform());

	// Draw children
	drawChildren(target, states);
}