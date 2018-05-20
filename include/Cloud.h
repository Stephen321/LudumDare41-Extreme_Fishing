#pragma once

#include "GameObject.h"
#include "Constants.h"


class Cloud : public GameObject, public sf::Drawable {
public:
	Cloud();
	Cloud(std::string cloudTexture);
	~Cloud();
	void update(float dt) override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
private:
	sf::Sprite m_sprite;
	float m_velocity_x;
	int m_currentIndex = 0;
	int m_previousIndex = 0;
	float LARGE_CLOUD_SPEED = 0.8f;
	float MEDIUM_CLOUD_SPEED = 0.6f;
	float SMALL_CLOUD_SPEED = 0.33f;
};