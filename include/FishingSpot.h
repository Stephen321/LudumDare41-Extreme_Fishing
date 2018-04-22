#pragma once

#include "GameObject.h"
#include "Spriter.h"
#include "Constants.h"

class FishingSpot : public GameObject, public sf::Drawable {
public:
	FishingSpot();
	void start(const sf::Vector2f start, int x, float timeToBeActive = FISHINGSPOT_MAXACTIVE);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	int getX() const;
	sf::IntRect getBoundingBox() const;
private:
	se::EntityInstance* m_entity;
	sf::Clock m_activeTimer;
	float m_timeToBeActive;
	int m_x;
};