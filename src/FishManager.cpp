#include "FishManager.h"
#include "Constants.h"

FishManager::FishManager(const sf::RenderWindow* _window)
	: window(_window) {
	for (int i = 0; i < MAX_FISHING_SPOTS; i++) {
		m_fishingSpots.emplace_back(FishingSpot());
	}
	for (int i = 0; i < TILES_X; i++) {
		m_possibleXLocations.push_back(i);
	}
	
}

void FishManager::start(){
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		m_fishingSpots[i].setAlive(false);
	}
	m_spawnTimer.restart();
}

void FishManager::update(float dt){
	std::vector<int> xActive;
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		if (m_fishingSpots[i].getAlive()) {
			m_fishingSpots[i].update(dt);
			xActive.push_back(m_fishingSpots[i].getX());
		}
	}
	if (xActive.size() < FISHINGSPOT_MAXACTIVE) {
		if (m_spawnTimer.getElapsedTime().asSeconds() > FISHINGSPOT_SPAWN_TIME) {
			m_spawnTimer.restart();
			if (rand() % FISHINGSPOT_SPAWN_CHANCE == 0) {
				int spawnSpot = -1;
				while (spawnSpot < 0) {
					spawnSpot = rand() % TILES_X;
					if (std::find(xActive.begin(), xActive.end(), spawnSpot) != xActive.end())
						spawnSpot = -1;
				}
				float x = spawnSpot * TILE_SIZE;
				//float bottom = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f);
				//float y = ((int)bottom - ((int)bottom % TILE_SIZE)) - (TILE_SIZE * WATER_TILES);
				float y = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f) - (TILE_SIZE * WATER_TILES);
				for (int i = 0; i < m_fishingSpots.size(); i++) {
					if (!m_fishingSpots[i].getAlive()) {
						m_fishingSpots[i].start(sf::Vector2f(x, y), x, FISHINGSPOT_ACTIVE_TIME);
						break;
					}
				}
			}
		}
	}
}

void FishManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		target.draw(m_fishingSpots[i]);
	}
}
