#include "ResourceManager.h"
#include <iostream>

std::map<std::string, sf::Texture*> ResourceManager::s_textureMap = std::map<std::string, sf::Texture*>();
std::map<std::string, sf::Font*> ResourceManager::s_fontMap = std::map<std::string, sf::Font*>();
std::map<std::string, sf::Shader*> ResourceManager::s_shaderMap = std::map<std::string, sf::Shader*>();

sf::Texture* ResourceManager::getTexture(const std::string& key) {
	sf::Texture* resource = getResource(s_textureMap, key);
	if (resource == nullptr) {
		// Create the resource
		resource = new sf::Texture;

		if (!resource->loadFromFile("../assets/textures/" + key + ".png")) {
			// If the above failed, try a different dir
			if (!resource->loadFromFile("assets/textures/" + key + ".png")) {
				// If that failed, the resource does not exist
				return nullptr;
			}
		}

		s_textureMap.emplace(std::make_pair(key, resource));
	}
	
	return resource;
}

sf::Font* ResourceManager::getFont(const std::string& key) {
	sf::Font* resource = getResource(s_fontMap, key);
	if (resource == nullptr) {
		// Create the resource
		resource = new sf::Font;

		if (!resource->loadFromFile("../assets/fonts/" + key + ".ttf")) {
			// If the above failed, try a different dir
			if (!resource->loadFromFile("assets/fonts/" + key + ".ttf")) {
				// If that failed, the resource does not exist
				return nullptr;
			}
		}

		s_fontMap.emplace(std::make_pair(key, resource));
	}

	return resource;
}

sf::Shader* ResourceManager::getShader(const std::string& key) {
	sf::Shader* resource = getResource(s_shaderMap, key);
	if (resource == nullptr) {
		// Create the resource
		resource = new sf::Shader;

		// DBG
		// std::cout.rdbuf(sf::err().rdbuf());

		if (!resource->loadFromFile("../assets/shaders/" + key + ".vert", "../assets/shaders/" + key + ".frag")) {
			// If the above failed, try a different dir
			if (!resource->loadFromFile("assets/shaders/" + key + ".vert", "assets/shaders/" + key + ".frag")) {
				// If that failed, the resource does not exist
				return nullptr;
			}
		}

		s_shaderMap.emplace(std::make_pair(key, resource));
	}

	return resource;
}

template<typename _Ty>
_Ty* ResourceManager::getResource(std::map<std::string, _Ty*>& resourceMap, const std::string& key) {
	auto it = resourceMap.find(key);
	if (it != resourceMap.end()) { return it->second; }
	return nullptr;
}

template sf::Texture* ResourceManager::getResource<sf::Texture>(std::map<std::string, sf::Texture*>&, const std::string&);
template sf::Font* ResourceManager::getResource<sf::Font>(std::map<std::string, sf::Font*>&, const std::string&);
template sf::Shader* ResourceManager::getResource<sf::Shader>(std::map<std::string, sf::Shader*>&, const std::string&);