#pragma once

#include "GameObject.h"
#include "Spriter.h"
#include "Constants.h"

class FishingSpot : public GameObject, public sf::Drawable {
public:
	FishingSpot();
	void start(const sf::Vector2f start, int x, int qteMax);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	int getX() const;
	sf::IntRect getBoundingBox() const;
	int getLength() const;
	float getTime() const;
	void setBeingFished(bool beingFished);
private:
	se::EntityInstance* m_entity;
	sf::Clock m_activeTimer;
	int m_timeToBeActive;
	int m_x;
	int m_length;
	float m_time;
	bool m_beingFished;
};