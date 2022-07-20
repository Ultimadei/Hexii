#include <iomanip>
#include <sstream>

#include "NumberHUD.h"
#include "ResourceManager.h"

NumberHUD::NumberHUD(SPBigNumber target, sf::Vector2f initialPosition) :
	m_text(std::make_shared<TextElement>(ResourceManager::getFont("finlandica"))),
	m_sprite(nullptr),
	m_centered(false),
	m_maxWidth(0.0f),
	m_maxHeight(0.0f),
	m_scaleSprite(false)
{
	m_text->setFillColor(sf::Color::Black);
	setPosition(initialPosition);
	setNumber(target); // This will also set the text

	setSubElement("text", m_text);
}

void NumberHUD::centerText(bool center) {
	m_centered = center;
	updateText();
}

void NumberHUD::scaleTextToFit(float maxWidth, float maxHeight, bool scaleSprite) {
	m_maxWidth = maxWidth;
	m_maxHeight = maxHeight;
	m_scaleSprite = scaleSprite;
	updateText();
}

void NumberHUD::setSprite(SPTexture texture, float scaleFactor) {
	m_sprite = std::make_shared<SpriteElement>(texture);

	const sf::FloatRect spriteBounds = m_sprite->getLocalBounds();

	// Set the sprite's origin to its local centre, before scaling
	m_sprite->setOrigin(spriteBounds.left + spriteBounds.width * 0.5f, spriteBounds.top + spriteBounds.height * 0.5f);
	m_sprite->setScale(scaleFactor, scaleFactor);

	updateSpritePosition();

	setSubElement("sprite", m_sprite);
}

void NumberHUD::setNumber(SPBigNumber number) {
	m_number = number;

	if (m_number.expired()) m_text->setText("");
	else updateText();
	
	// Update the sprite position, as the numberText may have a new size
	updateSpritePosition();
}

void NumberHUD::update(float dt) {
	SPBigNumber num = m_number.lock();

	// If the number hasn't changed, no update necessary
	if (*num == m_lastNumber) return;

	updateText();
	updateSpritePosition();
	m_lastNumber = *num;
}

void NumberHUD::updateText() {
	SPBigNumber num = m_number.lock();

	const sf::FloatRect bounds = m_text->getLocalBounds();

	// Update the text
	m_text->setText(bigNumberToString(*num, 3, !(mpf_integer_p(num->get_mpf_t()) && *num < 10000)));

	// Set the display's origin to its local centre. This has the effect of centering the text
	if (m_centered) {
		m_text->setOrigin(
			bounds.left + bounds.width * 0.5f,
			bounds.top + bounds.height * 0.5f
		);
	}

	if (m_maxWidth != 0.0f || m_maxHeight != 0.0f) {
		// Scale the display down (or up) to a maximum size
		float xSf = m_maxWidth / bounds.width;
		float ySf = m_maxHeight / bounds.height;

		// Use the smallest of the above for both axes
		float sf = std::min(xSf, ySf);

		m_text->setScale(sf, sf);
		if (m_scaleSprite && m_sprite != nullptr) m_sprite->setScale(sf, sf);
	}
}

void NumberHUD::updateSpritePosition() {
	if (m_sprite == nullptr) { return; }
	m_sprite->setPosition(55.0f + 34.0f * m_text->getText().size(), 45.0f);
}
