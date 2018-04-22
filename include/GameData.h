#pragma once

#include <unordered_map>
#include <cassert>
#include "SFML\Graphics\Texture.hpp"
#include "Spriter.h"


class GameData {
public:
	static GameData& getInstance();
	GameData(GameData const&) = delete;             
	GameData(GameData&&) = delete;                  
	GameData& operator=(GameData const&) = delete;
	GameData& operator=(GameData &&) = delete;

	~GameData();

	void load(sf::RenderWindow* _window);
	template<class T> T& getAsset(const std::string& name);
	template<class T> void addAsset(const char*  name, const char* path) {};
	template<> void addAsset<sf::Texture>(const char*  name, const char* path) { addTexture(name, path); }
	template<> void addAsset<se::SpriterModel>(const char*  name, const char* path) { addModel(name, path); }


private:
	struct BaseAsset {
		virtual ~BaseAsset() {}
	};
	template<class T> struct Asset : BaseAsset {
		T data;
	};
	template<class T> struct Asset<T*> : BaseAsset{
		~Asset() {
			if (data != nullptr)
				delete data;
		}
		T* data;
	};
	GameData();
	void addTexture(const char*  name, const char* path);
	void addModel(const char*  name, const char* path);
	std::unordered_map<std::string, BaseAsset*> assets;
	sf::RenderWindow* window;
};

template<class T>
inline T & GameData::getAsset(const std::string & name) {
	if (assets.find(name) != assets.end()) {
		Asset<T>* a = dynamic_cast<Asset<T>*>(assets.find(name)->second);
		if (a) {
			return a->data;
		}
	}
	assert(false && "couldnt find that asset!");
}
