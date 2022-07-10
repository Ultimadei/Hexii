#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

// TODO: Convert to singleton

class ResourceManager {
public:
	static sf::Texture* getTexture(const std::string& key);
	static sf::Font* getFont(const std::string& key);
	static sf::Shader* getShader(const std::string& key);
private:
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	template <typename _Ty>
	static _Ty* getResource(std::map<std::string, _Ty*>& resourceMap, const std::string& key);

	static std::map<std::string, sf::Texture*> s_textureMap;
	static std::map<std::string, sf::Font*> s_fontMap;
	static std::map<std::string, sf::Shader*> s_shaderMap;
};

// These lines are necessary to shut up the linker >:(
extern template sf::Texture* ResourceManager::getResource<sf::Texture>(std::map<std::string, sf::Texture*>&, const std::string&);
extern template sf::Font* ResourceManager::getResource<sf::Font>(std::map<std::string, sf::Font*>&, const std::string&);
extern template sf::Shader* ResourceManager::getResource<sf::Shader>(std::map<std::string, sf::Shader*>&, const std::string&);