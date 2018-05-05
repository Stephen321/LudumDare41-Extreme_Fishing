#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "Helpers.h"
using std::cout;
using std::endl;
using namespace Helpers;

class GameObject {
public:
	GameObject();
	bool getAlive() const;
	void setAlive(bool alive);
	virtual void update(float dt) = 0;
	sf::Vector2f getPosition() const;
protected:
	sf::Vector2f m_position;
	bool m_alive;
};
