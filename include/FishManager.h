#pragma once

#include <vector>
#include "FishingSpot.h"

class FishManager : public sf::Drawable{
public:
	FishManager(const sf::RenderWindow* _window);

	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	std::vector<FishingSpot> m_fishingSpots;
	std::vector<int> m_possibleXLocations;
	const sf::RenderWindow* window;
	sf::Clock m_spawnTimer;
};
