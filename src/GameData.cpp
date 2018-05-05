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
    addFont("font", "assets/font/BADABB.TTF");

    //add textures
    addTexture("menu", "assets/sprites/main/menu.png");
    addTexture("sky", "assets/sprites/sky.png");
    addTexture("title", "assets/sprites/main/titlelogo.png");
    addTexture("play", "assets/sprites/main/pressplay.png");
    addTexture("gameover", "assets/sprites/gameover/gameover.png");

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
