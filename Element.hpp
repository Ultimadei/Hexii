#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "ResourceManager.h"

// An `Element` is anything that's drawable and transformable. Basically anything to which a shader could apply
class Element :
	public sf::Drawable,
	public sf::Transformable {
public:
	using SPShader = ResourceManager::SPShader;
	using SPTexture = ResourceManager::SPTexture;
	using SPFont = ResourceManager::SPFont;

	typedef std::shared_ptr<Element> SPElement;

public:
	inline Element() : m_shader(nullptr) {}
	inline virtual ~Element() = 0 {}

	inline void setShader(SPShader shader) { m_shader = shader; }

	// Returns the sub element under `key`, or nullptr if none
	inline SPElement getSubElement(const std::string& key) const {
		auto it = m_subElements.find(key);
		if (it != m_subElements.end()) return it->second;
		else return nullptr;
	}

	template <typename _Ty, std::enable_if_t<std::is_base_of<Element, _Ty>::value>* = nullptr> 
	inline std::shared_ptr<_Ty> getSubElementAs(const std::string& key) const { return std::dynamic_pointer_cast<_Ty>(getSubElement(key)); }

	inline void setSubElement(const std::string& key, SPElement element) {
		auto it = m_subElements.find(key);
		if (it != m_subElements.end()) it->second = element;
		else m_subElements.emplace(std::make_pair(key, element));
	}

	bool active = true;

protected:
	inline virtual void drawSubElements(sf::RenderTarget& target, sf::RenderStates states) const { for (auto& element : m_subElements) target.draw(*element.second, states); }
	inline virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (!active) return;
		states.transform.combine(getTransform());
		drawSubElements(target, states);
	}

	SPShader m_shader;
	std::map<std::string, SPElement> m_subElements;
};