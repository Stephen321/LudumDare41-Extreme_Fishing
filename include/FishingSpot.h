#pragma once

#include "GameObject.h"
#include "Spriter.h"

class FishingSpot : public GameObject, sf::Drawable {
public:
	FishingSpot();
	void start(const sf::Vector2f start);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
	se::EntityInstance* m_entity;
	
};