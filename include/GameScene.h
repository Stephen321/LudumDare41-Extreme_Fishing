#pragma once

#include "Scene.h"
#include "Player.h"


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
	se::EntityInstance* m_entity;
	sf::CircleShape debugCircle;
};