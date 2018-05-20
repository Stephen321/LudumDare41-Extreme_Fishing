#pragma once

#include "GameObject.h"
#include "Constants.h"


class Cloud : public GameObject, public sf::Drawable {
public:
	Cloud();
	~Cloud();
	void update(float dt) override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
private:
	sf::Sprite m_sprite;
	float m_velocity_x;
};