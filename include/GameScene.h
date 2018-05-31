#pragma once

#include "Scene.h"
#include "Player.h"
#include "PlatformManager.h"
#include "FishManager.h"
#include "SkyManager.h"
#include "UIManager.h"
#include "Water.h"
#include "Island.h"
#include "SFML/Audio.hpp"


class GameScene : public Scene {
public:
	GameScene(sf::RenderWindow* _window);
	void start() override;
	void stop() override;
	void handleEvents(const sf::Event& ev) override;
	void update(float dt) override;
	void render(sf::RenderStates states) const override;

private:
	void autoScroll(float dt);

	Player m_player;
	PlatformManager m_platformManager;
	FishManager m_fishManager;
	SkyManager m_cloudManager;
	UIManager m_uiManager;
	Water m_water;
	Island m_island;
	sf::CircleShape debugCircle;
	sf::View m_view;
	sf::Sprite m_background;
	sf::Music music;
};