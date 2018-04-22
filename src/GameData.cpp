#include "GameData.h"
#include "Running.h"
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

	//add font
	addAsset<sf::Font>("font", "assets/font/BADABB.TTF");

	//add textures
	addAsset<sf::Texture>("menu", "assets/sprites/menu.png");
	addAsset<sf::Texture>("gameover", "assets/sprites/gameover.png");
	addAsset<sf::Texture>("sky", "assets/sprites/sky.png");
	addAsset<sf::Texture>("title", "assets/sprites/main/titlelogo.png");
	addAsset<sf::Texture>("play", "assets/sprites/main/pressplay.png");
	addAsset<sf::Texture>("gameover", "assets/sprites/gameover/gameover.png");

	//add animations
	addAsset<se::SpriterModel>("fisher", "assets/animations/Fisher/fisher.scon");
	addAsset<se::SpriterModel>("platform", "assets/animations/Platform/platform.scon");
	addAsset<se::SpriterModel>("fishingSpot", "assets/animations/FishingSpot/fishingSpot.scon");
	addAsset<se::SpriterModel>("water", "assets/animations/Ocean/water.scon");

	assert(assets.size() && "no asserts were loaded");
}

void GameData::addTexture(const char * name, const char * path) {
	if (!getRunning()) // only load if we are stilling running
		return;
	Asset<sf::Texture>* a = new Asset<sf::Texture>;
	a->data.loadFromFile(path);
	assets[std::string(name)] = a;
}

void GameData::addModel(const char * name, const char * path) {
	if (!getRunning())
		return;
	Asset<se::SpriterModel*>* a = new Asset<se::SpriterModel*>;
	a->data = new se::SpriterModel(path, new se::ExampleFileFactory(window), new se::ExampleObjectFactory(window));
	assets[std::string(name)] = a;
}

void GameData::addFont(const char * name, const char * path){
	Asset<sf::Font>* a = new Asset<sf::Font>;
	a->data.loadFromFile(path);
	assets[std::string(name)] = a;
}
