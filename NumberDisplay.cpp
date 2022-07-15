#include <iomanip>
#include <sstream>

#include "NumberDisplay.h"
#include "ResourceManager.h"

NumberDisplay::NumberDisplay(double initialNumber, sf::Vector2f initialPosition) :
	sf::Text("", *ResourceManager::getFont("finlandica")),
	m_number(initialNumber),
	m_sprite(nullptr)
{
	setFillColor(sf::Color::Black);
	setPosition(initialPosition);
	setNumber(initialNumber); // This will also set the text
}

NumberDisplay::~NumberDisplay() {
	if (m_sprite != nullptr) {
		delete m_sprite;
	}
}

void NumberDisplay::setSprite(sf::Sprite sprite, float scaleFactor) {
	if (m_sprite != nullptr) {
		delete m_sprite;
	}

	m_sprite = new sf::Sprite(sprite);

	const sf::FloatRect spriteLocalBounds = m_sprite->getLocalBounds();

	// Set the sprite's origin to its local centre, before scaling
	m_sprite->setOrigin(spriteLocalBounds.left + spriteLocalBounds.width * 0.5f, spriteLocalBounds.top + spriteLocalBounds.height * 0.5f);
	m_sprite->setScale(scaleFactor, scaleFactor);

	updateSpritePosition();
}

void NumberDisplay::setSprite(sf::Texture* texture, float scaleFactor) {
	setSprite(sf::Sprite(*texture), scaleFactor);
}

void NumberDisplay::setSprite(const std::string& textureName, float scaleFactor) {
	setSprite(ResourceManager::getTexture(textureName), scaleFactor);
}

const sf::Sprite* NumberDisplay::getSprite() const {
	return m_sprite;
}

void NumberDisplay::setNumber(const BigNumber& number) {
	m_number = number;

	// Set the numberText. Use fixed formatting unless the number is an integer less than 1000
	setString(bigNumberToString(m_number, 3, !(mpf_integer_p(m_number.get_mpf_t()) && m_number < 10000)));

	// Update the sprite position, as the numberText may have a new size
	updateSpritePosition();
}

BigNumber NumberDisplay::getNumber() const {
	return m_number;
}

void NumberDisplay::incrementNumber(const BigNumber& operand) {
	setNumber(m_number + operand);
}

void NumberDisplay::decrementNumber(const BigNumber& operand) {
	setNumber(m_number - operand);
}

void NumberDisplay::multiplyNumber(const BigNumber& operand, bool floorResult) {
	if (floorResult) {
		setNumber(floor(m_number * operand));
	}
	else {
		setNumber(m_number * operand);
	}
}

void NumberDisplay::divideNumber(const BigNumber& operand, bool floorResult) {
	if (floorResult) {
		setNumber(floor(m_number / operand));
	}
	else {
		setNumber(m_number / operand);
	}
}

void NumberDisplay::operator+=(const BigNumber& operand) {
	incrementNumber(operand);
}

void NumberDisplay::operator-=(const BigNumber& operand) {
	decrementNumber(operand);
}

void NumberDisplay::operator*=(const BigNumber& operand) {
	multiplyNumber(operand);
}

void NumberDisplay::operator/=(const BigNumber& operand) {
	divideNumber(operand);
}

void NumberDisplay::update(float dt) 
{}

void NumberDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(static_cast<sf::Text>(*this), states);

	states.transform.combine(getTransform());	

	if (m_sprite != nullptr) {
		target.draw(*m_sprite, states);
	}
}

void NumberDisplay::updateSpritePosition() {
	if (m_sprite == nullptr) { return; }

	const sf::Vector2f position = getPosition();
	const sf::FloatRect textBounds = getLocalBounds();
	const sf::FloatRect spriteGlobalBounds = m_sprite->getGlobalBounds();

	// Align the sprite's centre with the centre of the text string, then move the sprite to the right of the text
	m_sprite->setPosition(textBounds.left + textBounds.width * 0.5f, textBounds.top + textBounds.height * 0.5f);
	m_sprite->move(20.0f + textBounds.width * 0.5f + spriteGlobalBounds.width * 0.5f, 0.0f);
}
