#pragma once

#include "Scene.h"
#include "Player.h"
#include "PlatformManager.h"


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
	sf::CircleShape debugCircle;
	sf::View m_view;
	
	void autoScroll(float dt);
};