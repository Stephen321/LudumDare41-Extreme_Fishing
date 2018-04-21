#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>
using std::cout;
using std::endl;

class GameObject {
public:
	GameObject();
	bool getAlive() const;
	void setAlive(bool alive);
	virtual void update(float dt) = 0;
	sf::Vector2f getPosition();
protected:
	sf::Vector2f m_position;
	bool m_alive;
};
