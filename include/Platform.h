#pragma once

#include "GameObject.h"
#include "Spriter.h"

class Platform : public GameObject, public sf::Drawable {
public:
	Platform(const sf::Vector2f& start, const sf::RenderTarget* _window);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	sf::IntRect getBoundingBox() const;
private:
	se::EntityInstance* m_entity;
	sf::Vector2f m_velocity;
	float m_speed;
	bool m_grounded;
	const sf::RenderTarget* window;
};
