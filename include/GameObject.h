#pragma once

#include "SFML\Graphics.hpp"

class GameObject {
public:
	GameObject();
	bool getAlive() const;
	void setAlive(bool alive);
	virtual void update(float dt) = 0;
protected:
	sf::Vector2f m_position;
	bool m_alive;
};
