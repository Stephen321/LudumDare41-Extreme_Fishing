#include "FishManager.h"

#include <cmath>
#include "Constants.h"
#include "GameData.h"
#include "SceneManager.h"

FishManager::FishManager(const sf::RenderWindow* _window, Player* _player)
	: window(_window)
	, m_success(false)
	, playerPos(_player->getPositionPtr()) {
	for (int i = 0; i < MAX_FISHING_SPOTS; i++) {
		m_fishingSpots.emplace_back(FishingSpot());
	}
	for (int i = 0; i < MAX_SHARKS_ACTIVE; i++) {
		m_sharks.emplace_back(Shark(_player));
	}
	for (int i = 0; i < TILES_X; i++) {
		m_possibleXLocations.push_back(i);
	}

	m_fishingLine.setFillColor(sf::Color::White);
	m_fishingLine.setSize(sf::Vector2f(FISHINGLINE_THICKNESS, 1.f));
	m_fishingLine.setOrigin(FISHINGLINE_THICKNESS * 0.5f, 0.f);
	m_fishingLine.setPosition(-100.f, -100.f);
}

void FishManager::start(){
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		m_fishingSpots[i].setAlive(false);
	}
	for (int i = 0; i < m_sharks.size(); i++) {
		m_sharks[i].setAlive(false);
	}
	m_fishingLine.setPosition(-100.f, -100.f);
	m_spawnTimer.restart();
	m_timeNeededToAttempt = 0.f;
	m_success = false;
	m_playerQte = nullptr;
	m_playerLives = 5; //m
}

void FishManager::update(float dt){
	m_xActive.clear();

	m_allSharksActive = true;
	for (int i = 0; i < m_sharks.size(); i++) {
		m_sharks[i].update(dt);
		if (m_sharks[i].getAlive() == false) {
			m_allSharksActive = false;
		}
	}
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		if (m_fishingSpots[i].getAlive()) {
			m_fishingSpots[i].update(dt);
			if (m_fishingSpots[i].getIsShark()) {
				for (int j = 0; j < SHARK_SIZE_X; j++) {
					m_xActive.push_back(m_fishingSpots[i].getX() + j);
				}
				if (m_fishingSpots[i].justDied() && m_allSharksActive == false) {
					//shark spawn!
					int sharkToSpawn = -1;
					for (int j = 0; j < m_sharks.size(); j++) {
						if (m_sharks[j].getAlive() == false) {
							sharkToSpawn = j;
							break;
						}
					}
					if (sharkToSpawn != -1) {
						//there is a shark object that is not alive then we can spawn a shark !
						m_sharks[sharkToSpawn].start(
							sf::Vector2f(
								m_fishingSpots[i].getPosition().x + ((SHARK_SIZE_X / 2) * TILE_SIZE),
								m_fishingSpots[i].getPosition().y + (WATER_TILES * TILE_SIZE)
							), *playerPos);
						m_fishingSpots[i].setX(-100.f);
					}
				}
			}
			else {
				m_xActive.push_back(m_fishingSpots[i].getX());
			}
		}
	}
	float currentWaterLevel= window->getView().getCenter().y + WATER_Y_OFFSET;
	if (m_xActive.size() < FISHINGSPOT_MAXACTIVE) {
		if (m_spawnTimer.getElapsedTime().asSeconds() > FISHINGSPOT_SPAWN_TIME) {
			m_spawnTimer.restart();
			int spawnSpot = -1;
			bool shark = false;
			if (m_allSharksActive == false) {
				if (rand() % 2) { //spawn a fish spot
					if (rand() % FISHINGSPOT_SPAWN_CHANCE == 0) {
						while (spawnSpot < 0) {
							spawnSpot = rand() % TILES_X;
							if (std::find(m_xActive.begin(), m_xActive.end(), spawnSpot) != m_xActive.end())
								spawnSpot = -1;
						}
					}
				}
				else if (rand() % SHARKSPOT_SPAWN_CHANCE == 0) { //spawn a shark!
					const int MAX_LOOPS = 1000; //if we couldnt spawn a shark after this many loops then there must be no available space currently..
					int m_loop = 0;
					while (spawnSpot < 0 || m_loop > MAX_LOOPS) {
						spawnSpot = rand() % (TILES_X - SHARK_SIZE_X);
						if (std::find(m_xActive.begin(), m_xActive.end(), spawnSpot) != m_xActive.end())
							spawnSpot = -1; //dont choose a spot that has a fishing spot active
						else {
							//make sure it wont overlap with  an active fishing spot
							for (int i = 0; i < SHARK_SIZE_X; i++) {
								if (std::find(m_xActive.begin(), m_xActive.end(), spawnSpot + i) != m_xActive.end()) {
									spawnSpot = -1;
									break;
								}
							}
						}
						m_loop++;
					}
					if (spawnSpot != -1)
						shark = true;
				}
			}
			else { //spawn a fish spot
				if (rand() % FISHINGSPOT_SPAWN_CHANCE == 0) {
					while (spawnSpot < 0) {
						spawnSpot = rand() % TILES_X;
						if (std::find(m_xActive.begin(), m_xActive.end(), spawnSpot) != m_xActive.end())
							spawnSpot = -1;
					}
				}
			}
			if (spawnSpot != -1) {
				float x = spawnSpot * TILE_SIZE;
				for (int i = 0; i < m_fishingSpots.size(); i++) {
					if (!m_fishingSpots[i].getAlive()) {
						int qteMax = QTE_MAX; //todo: change the max depending on how long the player survives for
						m_fishingSpots[i].start(sf::Vector2f(x, currentWaterLevel + FISHINGSPOT_Y_OFFSET), spawnSpot, qteMax, shark);
						break;
					}
				}
			}
		}
	}
	if (m_playerQte && *m_playerQte == false) {
		if (m_success) {
			m_success = false;
			m_fishingLine.setPosition(-100.f, -100.f);
		}
		if (m_fishedSpot != -1) {
			m_fishingSpots[m_fishedSpot].setDying();
			m_fishedSpot = -1;
		}
	}
}

void FishManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_fishingLine);
	for (int i = 0; i < m_sharks.size(); i++) {
		target.draw(m_sharks[i]);
	}
	for (int i = 0; i < m_fishingSpots.size(); i++) {
		target.draw(m_fishingSpots[i]);
	}
	//sf::RectangleShape d;
	//d.setFillColor(sf::Color(0, 255, 0, 100));
	//d.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	//for (int i = 0; i < m_xActive.size(); i++) {
	//	float x = m_xActive[i] * TILE_SIZE;
	//	d.setPosition(x, m_water->getPosition().y);
	//	target.draw(d);
	//}
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
		bool fishingSpotExists = false;
		int lineX = m_fishingLine.getPosition().x - ((int)m_fishingLine.getPosition().x % TILE_SIZE);
		lineX /= TILE_SIZE;
		for (int i = 0; i < m_fishingSpots.size(); i++) {
			if (m_fishingSpots[i].getIsShark() == false && m_fishingSpots[i].getAlive() && m_fishingSpots[i].getX() == lineX) {
				m_fishedSpot = i;
				fishingSpotExists = true;
				break;
			}
		}
		if (fishingSpotExists) {
			assert(m_fishedSpot != -1);
			//success
			m_success = true;
			m_timer.restart();
			m_fishingSpots[m_fishedSpot].setBeingFished(true);
			player->setSuccessfulAttempt(m_fishingSpots[m_fishedSpot].getLength(), m_fishingSpots[m_fishedSpot].getTime());
			m_playerQte = player->getQte();
		}
		else {//fail
			player->setFailedAttempt();
			m_fishingLine.setPosition(-100.f, -100.f);
		}
	}
}

sf::Vector2f FishManager::getQteFishSpot() const {
	if (m_fishedSpot != -1 && *m_playerQte) { //already done in gamescene
		return m_fishingSpots[m_fishedSpot].getPosition();
	}
	return sf::Vector2f();
}
