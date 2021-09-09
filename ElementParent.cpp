#include "ElementParent.h"

ElementParent::ElementParent(ElementParent&& right) noexcept :
	ParentBase(reinterpret_cast<ParentBase&&>(right)) 
{}

bool ElementParent::setElement(const std::string& name, ElementBase* child, bool conditional) {
	return setChild(name, child, conditional);
}

const ElementBase* ElementParent::getElement(const std::string& name) const {
	return getChild(name);
}

ElementBase* ElementParent::accessElement(const std::string& name) {
	return accessChild(name);
}

void ElementParent::removeElement(const std::string& name) {
	removeChild(name);
}

void ElementParent::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto i : m_children) {
		target.draw(*i.second, states);
	}
}