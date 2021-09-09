#pragma once

#include <SFML/Graphics.hpp>

#include "BigNumber.h"

// Displays a number as text and can have a sprite icon
class NumberDisplay :
	public sf::Text {
public:
	NumberDisplay(double initialNumber = 0.0, sf::Vector2f initialPosition = sf::Vector2f(0.0f, 0.0f));
	~NumberDisplay();

	void setSprite(sf::Sprite sprite, float scaleFactor = 1.0f);
	// Construct the sprite from a texture
	void setSprite(sf::Texture* texture, float scaleFactor = 1.0f);
	// Grab the texture and construct the sprite from it
	void setSprite(const std::string& textureName, float scaleFactor = 1.0f);
	const sf::Sprite* getSprite() const;

	void setNumber(const BigNumber& number);
	BigNumber getNumber() const;

	void incrementNumber(const BigNumber& operand);
	void decrementNumber(const BigNumber& operand);
	void multiplyNumber(const BigNumber& operand, bool floorResult = false);
	void divideNumber(const BigNumber& operand, bool floorResult = false);

	void operator+=(const BigNumber& operand);
	void operator-=(const BigNumber& operand);
	void operator*=(const BigNumber& operand);
	void operator/=(const BigNumber& operand);

	void update(float dt);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// Sets the sprite position to be to the right of the text
	void updateSpritePosition();

	BigNumber m_number;

	sf::Sprite* m_sprite;
};

