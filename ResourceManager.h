#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager {
public:
	typedef std::shared_ptr<sf::Texture> SPTexture;
	typedef std::shared_ptr<sf::Font> SPFont;
	typedef std::shared_ptr<sf::Shader> SPShader;

public:
	inline static SPTexture getTexture(const std::string& key) { return getResource(instance()->m_textureMap, "assets/textures", key, "png"); }
	inline static SPFont getFont(const std::string& key) { return getResource(instance()->m_fontMap, "assets/fonts", key, "ttf"); }
	inline static SPShader getShader(const std::string& key) { return getResource(instance()->m_shaderMap, "assets/shaders", key, ""); }

private:
	inline ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	static ResourceManager* s_instance;

	inline static ResourceManager* instance() {
		if (ResourceManager::s_instance == nullptr) ResourceManager::s_instance = new ResourceManager();
		return ResourceManager::s_instance;
	}

	/*
	template <typename _Ty>
	inline static std::shared_ptr<_Ty> getResource(std::map<std::string, std::shared_ptr<_Ty>>& resourceMap, const std::string& key) {
		auto it = resourceMap.find(key);
		if (it != resourceMap.end()) { return it->second; }
		return nullptr;
	}
	*/

	template <typename _Ty>
	inline static std::shared_ptr<_Ty> getResource(std::map<std::string, std::shared_ptr<_Ty>>& map, const std::string& path, const std::string& key, const std::string& ext) {
		auto it = map.find(key);
		if (it != map.end()) return it->second;
		
		std::shared_ptr<_Ty> resource = std::make_shared<_Ty>();
		if (!loadFromFile<_Ty>(*resource, path, key, ext)) return nullptr;

		map.emplace(std::make_pair(key, resource));

		return resource;
	}

	template <typename _Ty>
	inline static bool loadFromFile(_Ty& resource, const std::string& path, const std::string& key, const std::string& ext) {
		const std::string filename = path + "/" + key + "." + ext;
		if (resource.loadFromFile(filename)) return true;
		if (resource.loadFromFile("../" + filename)) return true;
		return false;
	}

	template <>
	inline static bool loadFromFile<sf::Shader>(sf::Shader& resource, const std::string& path, const std::string& key, const std::string& ext) {
		const std::string vertexFilename = path + "/" + key + ".vert";
		const std::string fragmentFilename = path + "/" + key + ".frag";
		if (resource.loadFromFile(vertexFilename, fragmentFilename)) return true;
		if (resource.loadFromFile("../" + vertexFilename, "../" + fragmentFilename)) return true;
		return false;
	}

	std::map<std::string, SPTexture> m_textureMap;
	std::map<std::string, SPFont> m_fontMap;
	std::map<std::string, SPShader> m_shaderMap;
};

/*
// These lines are necessary to shut up the linker >:(
extern template sf::Texture* ResourceManager::getResource<sf::Texture>(std::map<std::string, sf::Texture*>&, const std::string&);
extern template sf::Font* ResourceManager::getResource<sf::Font>(std::map<std::string, sf::Font*>&, const std::string&);
extern template sf::Shader* ResourceManager::getResource<sf::Shader>(std::map<std::string, sf::Shader*>&, const std::string&);
*/