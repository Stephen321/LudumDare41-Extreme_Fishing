#pragma once

#include "SFML\Graphics.hpp"
#include "Spriter.h"
#include "Constants.h"

class Water : public sf::Drawable {
public:
	Water(const sf::RenderWindow* _window);
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	int getLevel() const;
private:
	struct Spring {
		sf::Vector2f position; 
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
	};
	se::EntityInstance* m_entity;
	const sf::RenderWindow* window;
	mutable sf::VertexArray m_vertices;
	float m_level;
	Spring m_springs[WATER_SPRINGS_COUNT];
};