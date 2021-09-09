#pragma once

#include "AbstractElementBase.h"

// Interface for a real 'Element', which is a transformable which can be drawn to the screen
class ElementBase :
	public AbstractElementBase,
	public sf::Transformable {
public:
	inline ElementBase() {}
	virtual inline ~ElementBase() = 0 {}
};