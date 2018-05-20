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
	
    //add font
    addFont("font", "assets/font/BADABB.TTF");

    //add textures
    addTexture("menu", "assets/sprites/main/menu.png");
    addTexture("sky", "assets/sprites/sky.png");
	addTexture("cloud1", "assets/sprites/clouds/Cloud1.png");
	addTexture("cloud2", "assets/sprites/clouds/Cloud2.png");
	addTexture("cloud3", "assets/sprites/clouds/Cloud3.png");
	addTexture("cloud4", "assets/sprites/clouds/Cloud4.png");
	addTexture("cloud5", "assets/sprites/clouds/Cloud5.png");
	addTexture("cloud6", "assets/sprites/clouds/Cloud6.png");
	addTexture("cloud7", "assets/sprites/clouds/Cloud7.png");
	addTexture("cloud8", "assets/sprites/clouds/Cloud8.png");
    addTexture("title", "assets/sprites/main/titlelogo.png");
    addTexture("play", "assets/sprites/main/pressplay.png");
	addTexture("gameover", "assets/sprites/gameover/gameover.png");
	addTexture("particle", "assets/sprites/particle.png");
    addShader("blur", "assets/shaders/blur.frag", sf::Shader::Type::Fragment);

    //add animations
    addModel("fisher", "assets/animations/Fisher/fisher.scon");
    addModel("Shark", "assets/animations/Shark/Shark.scon");
    addModel("platform", "assets/animations/Platform/platform.scon");
    addModel("fishingSpot", "assets/animations/FishingSpot/fishingSpot.scon");
    addModel("SharkSpot", "assets/animations/SharkSpot/SharkSpot.scon");
    addModel("water", "assets/animations/Ocean/water.scon");
    addModel("QTE", "assets/animations/QTE/QTE.scon");

    assert(assets.size() && "no asserts were loaded");
}

void GameData::addShader(const char * name, const char * path, sf::Shader::Type type) {
	//TODO: what if you want multiple instances of a shader
	if (sf::Shader::isAvailable()) {
		Asset<sf::Shader>* a = new Asset<sf::Shader>;
		a->data.loadFromFile(path, type);
		assets[std::string(name)] = a;
	}
	else {
		std::cout << "Shaders not available" << std::endl;
	}
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
