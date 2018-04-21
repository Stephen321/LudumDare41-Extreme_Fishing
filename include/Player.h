#pragma once

#include "GameObject.h"
#include "Spriter.h"
#include "Platform.h"

class Player : public GameObject, public sf::Drawable {
public:
	Player();
	void start(const sf::Vector2f& start);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void checkCollisions(const std::vector<Platform>& platforms);
	sf::IntRect getBoundingBox() const;
private:
	se::EntityInstance* m_entity;
	sf::Vector2f m_velocity;
	sf::Clock m_spaceTimer; 
	bool m_spaceHeld;
	float m_speed;
	bool m_grounded;
	float m_lastY;
};
