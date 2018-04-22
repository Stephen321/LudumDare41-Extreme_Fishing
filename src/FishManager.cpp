#include "FishManager.h"
#include "Constants.h"
#include "GameData.h"

FishManager::FishManager(const sf::RenderWindow* _window)
	: window(_window)
	, m_success(false) {
	for (int i = 0; i < MAX_FISHING_SPOTS; i++) {
		m_fishingSpots.emplace_back(FishingSpot());
	}
	for (int i = 0; i < TILES_X; i++) {
		m_possibleXLocations.push_back(i);
	}
	m_water = GameData::getInstance().getAsset<se::SpriterModel*>("water")->getNewEntityInstance("Water");
	m_water->setCurrentAnimation("Idle");

	m_fishingLine.setFillColor(sf::Color::White);
	m_fishingLine.setSize(sf::Vector2f(FISHINGLINE_THICKNESS, 1.f));
	m_fishingLine.setOrigin(FISHINGLINE_THICKNESS * 0.5f, 0.f);
	m_fishingLine.setPosition(-100.f, -100.f);
}

void FishManager::start(){
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		m_fishingSpots[i].setAlive(false);
	}
	m_spawnTimer.restart();
	m_timeNeededToAttempt = 0.f;
	m_playerQte = false;
	m_success = false;
}

void FishManager::update(float dt){
	m_xActive.clear();
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		if (m_fishingSpots[i].getAlive()) {
			m_fishingSpots[i].update(dt);
			m_xActive.push_back(m_fishingSpots[i].getX());
		}
	}
	float currentWaterLevel= window->getView().getCenter().y + WATER_Y_OFFSET;
	if (m_xActive.size() < FISHINGSPOT_MAXACTIVE) {
		if (m_spawnTimer.getElapsedTime().asSeconds() > FISHINGSPOT_SPAWN_TIME) {
			m_spawnTimer.restart();
			if (rand() % FISHINGSPOT_SPAWN_CHANCE == 0) {
				int spawnSpot = -1;
				while (spawnSpot < 0) {
					spawnSpot = rand() % TILES_X;
					if (std::find(m_xActive.begin(), m_xActive.end(), spawnSpot) != m_xActive.end())
						spawnSpot = -1;
				}
				float x = spawnSpot * TILE_SIZE;
				//float bottom = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f);
				//float y = ((int)bottom - ((int)bottom % TILE_SIZE)) - (TILE_SIZE * WATER_TILES);
				for (int i = 0; i < m_fishingSpots.size(); i++) {
					if (!m_fishingSpots[i].getAlive()) {
						m_fishingSpots[i].start(sf::Vector2f(x, currentWaterLevel), x, FISHINGSPOT_ACTIVE_TIME);
						break;
					}
				}
			}
		}
	}
	if (m_success && *m_playerQte == false) {
		m_success = false;
		m_fishingLine.setPosition(-100.f, -100.f);
	}
	m_water->setPosition(se::point(0.f, currentWaterLevel));
	m_water->setTimeElapsed(dt);
}

void FishManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		target.draw(m_fishingSpots[i]);
	}
	target.draw(m_fishingLine);
	m_water->render();
}

void FishManager::attempt(Player* player) {
	//sf::Vector2f pp = player->getPosition();
	sf::IntRect bb = player->getBoundingBox();
	float currentWaterLevel = window->getView().getCenter().y + WATER_Y_OFFSET;
	float distance = currentWaterLevel + (TILE_SIZE * 0.5f) - bb.top;

	if (m_fishingLine.getPosition().x < 0.f && m_fishingLine.getPosition().y < 0.f) {
		//first time in here
		m_timer.restart();
		m_fishingLine.setPosition(player->getRodEnd());
		m_timeNeededToAttempt = distance / (FISHINGLINE_TIME_DIST / FISHINGLINE_TIME);
	}
	//line "throw" 
	float fishingTimeSec = m_timer.getElapsedTime().asSeconds();
	float m_fishingLineTime = FISHINGLINE_TIME * (fmin(distance / FISHINGLINE_TIME_DIST, FISHINGLINE_TIME_DIST));
	if (fishingTimeSec < m_fishingLineTime) {
		if (fishingTimeSec < 0.001f)
			fishingTimeSec = 0.001f;
		float s = (fishingTimeSec / m_fishingLineTime) * distance;
		m_fishingLine.setScale(1.f, s);
	}
	if (fishingTimeSec > m_timeNeededToAttempt) {
		//success
		m_success = true;
		m_timer.restart();
		player->setSuccessfulAttempt();
		m_playerQte = player->getQte();
		
		//fail
		//m_fishingLine.setPosition(-100.f, -100.f);
	}
}
