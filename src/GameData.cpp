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

	//add model asset and entities in it 
	std::vector<std::string> entities;
	entities.push_back("Player");
	addModelAsset("fisher", "assets/animations/Fisher/fisher.scon", entities);

	entities.clear();
	//entities.push_back("Platform");
	//no point adding entity to assets map cause you dont need just one..
	addModelAsset("platform", "assets/animations/Platform/platform.scon", entities);

	entities.push_back("FishingSpot");
	addModelAsset("fishingSpot", "assets/animations/FishingSpot/fishingSpot.scon", entities);

	assert(assets.size() && "no asserts were loaded");
}

void GameData::addTexture(const char * name, const char * path) {
	Asset<sf::Texture>* a = new Asset<sf::Texture>;
	a->data.loadFromFile(path);
	assets[std::string(name)] = a;
}

void GameData::addModelAsset(const char * name, const char * path, const std::vector<std::string>& entities) {
	Asset<se::SpriterModel*>* a = new Asset<se::SpriterModel*>;
	a->data = new se::SpriterModel(path, new se::ExampleFileFactory(window), new se::ExampleObjectFactory(window));
	assets[std::string(name)] = a;

	//note, this only stores 1 enitity instance, if you need more than one get the model and call getNewEntityInstance on it
	for (int i = 0; i < entities.size(); i++) {
		Asset<se::EntityInstance*>* e = new Asset<se::EntityInstance*>;
		e->data = a->data->getNewEntityInstance(entities[i]);
		assets[entities[i]] = e;
	}
}

void GameData::addEntity(const char * model, const char * name) {
	//getAsset<SpriterEngine::SpriterModel>
}
