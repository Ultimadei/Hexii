#pragma once

#include "ParentBase.h"
#include "ElementBase.h"

// Supports having an arbitrary number of drawable children ('Elements')
class ElementParent :
	public ParentBase<ElementBase> {
public:
	inline ElementParent() {}
	ElementParent(ElementParent&& right) noexcept;
	ElementParent(const ElementParent& right) = delete;

	bool setElement(const std::string& name, ElementBase* child, bool conditional = false);
	const ElementBase* getElement(const std::string& name) const;
	ElementBase* accessElement(const std::string& name);
	void removeElement(const std::string& name);
protected:
	virtual void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
};