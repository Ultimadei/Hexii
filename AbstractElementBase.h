#pragma once

#include <SFML/Graphics.hpp>

// Interface for an 'Abstract Element', which is anything that can be drawn to the screen
class AbstractElementBase :
	public sf::Drawable {
public:
	AbstractElementBase();

	virtual inline ~AbstractElementBase() = 0 {}

	virtual void setFillColor(const sf::Color& fill) = 0;
	virtual void setOutlineColor(const sf::Color& fill) = 0;
	virtual void setOutlineThickness(float thickness) = 0;
	virtual void setTexture(const sf::Texture* texture, bool resetRect = true) = 0;
	virtual void setTextureRect(const sf::IntRect& rect) = 0;
	void setShader(sf::Shader* shader);
	void setActive(bool active);

	virtual sf::Color getFillColor() const = 0;
	virtual sf::Color getOutlineColor() const = 0;
	virtual float getOutlineThickness() const = 0;
	virtual const sf::Texture* getTexture() const = 0;
	virtual sf::IntRect getTextureRect() const = 0;
	bool getActive() const;

	void operator=(const AbstractElementBase& right);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	sf::Shader* m_shader;
	bool m_active;
private:
};