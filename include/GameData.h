#pragma once

#include <unordered_map>
#include <cassert>
#include "SFML/Graphics/Texture.hpp"
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

    int score;
    int lives;
    int time;
    float top;


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
    void addFont(const char* name, const char* path);
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
    //https://stackoverflow.com/questions/5286922/g-template-parameter-error
    //TODO: try fix the error in gcc for this 
    //assert(false && "couldnt find that asset!");
}
