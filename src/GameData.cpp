#include "GameData.h"
#include <cassert>


GameData::GameData() {

}


GameData& GameData::getInstance() {
	static GameData gd;
	return gd;
}

GameData::~GameData() {
	for (auto& a : assets) {
		delete a.second;
	}
}

void GameData::load(sf::RenderWindow* _window) {
	window = _window;
	assert(window && "should always be a window when load is called");

	//Add assets here
	//--

	//add textures
	addAsset<sf::Texture>("menu", "assets/sprites/menu.png");
	addAsset<sf::Texture>("gameover", "assets/sprites/gameover.png");

	addAsset<se::SpriterModel>("fisher", "assets/animations/Fisher/fisher.scon");
	addAsset<se::SpriterModel>("platform", "assets/animations/Platform/platform.scon");
	addAsset<se::SpriterModel>("fishingSpot", "assets/animations/FishingSpot/fishingSpot.scon");

	assert(assets.size() && "no asserts were loaded");
}

void GameData::addTexture(const char * name, const char * path) {
	Asset<sf::Texture>* a = new Asset<sf::Texture>;
	a->data.loadFromFile(path);
	assets[std::string(name)] = a;
}

void GameData::addModel(const char * name, const char * path) {
	Asset<se::SpriterModel*>* a = new Asset<se::SpriterModel*>;
	a->data = new se::SpriterModel(path, new se::ExampleFileFactory(window), new se::ExampleObjectFactory(window));
	assets[std::string(name)] = a;
}