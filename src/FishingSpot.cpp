#include "FishingSpot.h"
#include "GameData.h"
#include "Constants.h"
#include "Helpers.h"

using namespace Helpers;

FishingSpot::FishingSpot(){
	m_alive = false;
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("fishingSpot")->getNewEntityInstance("FishingSpot");
}

void FishingSpot::start(const sf::Vector2f start, int x, int qteMax) {
	m_position = start;
	m_x = x;
	m_entity->setCurrentAnimation("Bubbles");
	m_activeTimer.restart();
	m_alive = true;
	m_beingFished = false;
	m_length = randomNumber(QTE_MIN, qteMax);
	double r = (double)rand() / RAND_MAX; //0 - 1.f
	float chance = 0.05f;
	for (int i = qteMax - QTE_MIN; i >= 0 ; i--) {
		if (r < chance)
			m_length = QTE_MIN + i;
		chance += 0.15f;
	}//length is more likely to be lower number

	m_time = ((m_length / ((QTE_MIN + QTE_MAX) * 0.5f)) * QTE_MIN_TIME) + randomNumberF(-QTE_RAND_OFFSET * 0.35f, QTE_RAND_OFFSET * 2.5f);
	m_timeToBeActive = (m_time * 1.5f) + randomNumberF(-FISHINGSPOT_ACTIVE_TIME * 0.75f, FISHINGSPOT_ACTIVE_TIME * 1.25f);
	m_timeToBeActive = fmax(m_timeToBeActive, FISHINGSPOT_ACTIVE_TIME);
	cout << "m_length: " << m_length;
	cout << ", m_time: " << m_time;
	cout << ", m_timeToBeActive: " << m_timeToBeActive << endl;
}

void FishingSpot::update(float dt) {
	m_position.y += SCROLL_SPEED * dt;
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);	
	if (m_activeTimer.getElapsedTime().asSeconds() > m_timeToBeActive && m_beingFished == false)
		m_alive = false;
}

void FishingSpot::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (!m_alive)
		return;
	m_entity->render();
}

int FishingSpot::getX() const {
	return m_x;
}

sf::IntRect FishingSpot::getBoundingBox() const {
	sf::IntRect bb;
	bb.left = (int)m_position.x;
	bb.top = (int)m_position.y;
	bb.width = FISHINGSPOT_SIZE_X * TILE_SIZE;
	bb.height = (int)(FISHINGSPOT_SIZE_Y * TILE_SIZE);
	return bb;
}

int FishingSpot::getLength() const {
	return m_length;
}

float FishingSpot::getTime() const {
	return m_time;
}

void FishingSpot::setBeingFished(bool beingFished) {
	m_beingFished = beingFished;
}
