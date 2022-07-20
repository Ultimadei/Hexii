#pragma once

#include "Element.hpp"
#include "TextElement.hpp"
#include "SpriteElement.hpp"
#include "BigNumber.h"

// TODO: Implement a TextElement class and refactor accordingly

// Displays a number as text and can have a sprite icon
class NumberHUD :
	public Element {
public:
	using SPTextElement = TextElement::SPTextElement;
	using SPSpriteElement = SpriteElement::SPSpriteElement;

public:
	// Tracks `target`. Text is centered at the position of NumberHUD if `centered` is true
	NumberHUD(SPBigNumber target, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f));

	// Configures alignment settings. Will automatically apply whenever the target number changes 
	void centerText(bool center = true);
	// Configures scaling settings. Will automatically apply whenever the target number changes
	// Max of 0 for both width and height will disable scaling
	void scaleTextToFit(float maxWidth = 0.0f, float maxHeight = 0.0f, bool scaleSprite = false);

	// Construct the sprite from a texture
	void setSprite(SPTexture texture, float scaleFactor = 1.0f);
	void setNumber(SPBigNumber number);

	SPTextElement getText() const { return m_text; }
	SPSpriteElement getSprite() const { return m_sprite; }
	WPBigNumber getNumber() const { return m_number; }

	void update(float dt);
private:
	// Set the numberText. Use fixed formatting unless the number is an integer less than 10000
	void updateText();

	// Sets the sprite position to be to the right of the text
	void updateSpritePosition();

	SPTextElement m_text;
	SPSpriteElement m_sprite;
	// Stored to compare if number changed since the last update
	BigNumber m_lastNumber;
	WPBigNumber m_number;

	bool m_centered;

	float m_maxWidth;
	float m_maxHeight;
	bool m_scaleSprite;
};

