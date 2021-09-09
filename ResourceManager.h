#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager {
public:
	static sf::Texture* getTexture(const std::string& key);
	static sf::Font* getFont(const std::string& key);
private:
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	template <typename _Ty>
	static _Ty* getResource(std::map<std::string, _Ty*>& resourceMap, const std::string& key);

	static std::map<std::string, sf::Texture*> s_textureMap;
	static std::map<std::string, sf::Font*> s_fontMap;
};

extern template sf::Texture* ResourceManager::getResource<sf::Texture>(std::map<std::string, sf::Texture*>&, const std::string&);
extern template sf::Font* ResourceManager::getResource<sf::Font>(std::map<std::string, sf::Font*>&, const std::string&);