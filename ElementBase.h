#pragma once

#include "AbstractElementBase.h"

// TODO: No need to have ElementBase and AbstractElementBase...

// Interface for a real 'Element', which is a transformable that can be drawn to the screen
class ElementBase :
	public AbstractElementBase,
	public sf::Transformable {
public:
	inline ElementBase() {}
	virtual inline ~ElementBase() = 0 {}
};