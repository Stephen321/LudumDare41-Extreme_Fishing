#pragma once

#include "Scene.h"
#include "Player.h"
#include "PlatformManager.h"
#include "FishManager.h"
#include "UIManager.h"


class GameScene : public Scene {
public:
	GameScene(sf::RenderWindow* _window);
	void start() override;
	void stop() override;
	void handleEvents(const sf::Event& ev) override;
	void update(float dt) override;
	void render(sf::RenderStates states) const override;

private:
	Player m_player;
	PlatformManager m_platformManager;
	FishManager m_fishManager;
	UIManager m_uiManager;
	sf::CircleShape debugCircle;
	sf::View m_view;
	sf::Sprite m_background;
	
	void autoScroll(float dt);
};