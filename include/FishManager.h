#pragma once

#include "SFML\Graphics.hpp"
#include "Constants.h"
#include <vector>

class FishManager {
public:
	FishManager();

	void start();
	void update(float dt);
	void draw();

private:
	std::vector<sf::Vector2f> m_fishingSpots;

	
};
