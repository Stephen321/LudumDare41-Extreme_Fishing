#pragma once

#include <vector>
#include "FishingSpot.h"
#include "FishManager.h"
#include "Player.h"

class FishManager : public sf::Drawable{
public:
	FishManager(const sf::RenderWindow* _window);

	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void attempt(Player* player);
private:
	std::vector<FishingSpot> m_fishingSpots;
	std::vector<int> m_possibleXLocations;
	const sf::RenderWindow* window;
	sf::Clock m_spawnTimer;
	se::EntityInstance* m_water;
	sf::RectangleShape m_fishingLine;
	sf::Clock m_timer;
	const bool* m_playerQte;
	bool m_success;
	float m_timeNeededToAttempt;
	int m_fishedSpot;
	std::vector<int> m_xActive;
};
