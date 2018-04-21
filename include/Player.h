#pragma once

#include "GameObject.h"
#include "Spriter.h"

class Player : public GameObject, sf::Drawable {
public:
	Player();
	void start(const sf::Vector2f start);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
	se::EntityInstance* m_entity;
	sf::Vector2f m_velocity;
	float m_speed;
	bool m_grounded;
};
