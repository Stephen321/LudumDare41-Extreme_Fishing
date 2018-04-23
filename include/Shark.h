#pragma once

#include "GameObject.h"
#include "Spriter.h"
#include "Constants.h"

class Shark : public GameObject, public sf::Drawable {
public:
	Shark();
	void start(const sf::Vector2f& start, sf::Vector2f target);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	sf::IntRect getBoundingBox() const;
	float* getColRads() const;
	sf::Vector2f* getColCentres() const;
private:
	se::EntityInstance* m_entity;
	float m_colRads[SHARK_COL_CIRCLES];
	sf::Vector2f m_colCentres[SHARK_COL_CIRCLES];
	sf::Vector2f m_velocity;
	float m_angle;
	float m_speed;
};
