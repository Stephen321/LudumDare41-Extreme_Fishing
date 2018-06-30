#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Spriter.h"


class Island : public GameObject, public sf::Drawable {
public:
	Island();
	void update(float dt) override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
private:
	sf::Sprite m_island_sprite;
	sf::Sprite m_sprite;
	se::EntityInstance* m_entity;
};