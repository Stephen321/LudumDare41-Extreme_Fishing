#pragma once

#include "GameObject.h"
#include "Constants.h"


class GodRay : public GameObject, public sf::Drawable {
public:
	GodRay();
	GodRay(std::string spriteName, float position_x, float easeDuration);
	void update(float dt) override;
	void fadeInOut();
	float quadraticEaseInOut(float t, float b, float c, float d);
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
private:
	sf::Sprite m_sprite;
	float m_alpha = 0;
	float m_ease = 2;
	float m_timer = 0;
	const float EASE_DURATION;
};