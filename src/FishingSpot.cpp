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
	m_length = randomNumber(QTE_MIN, qteMax);
	m_time = ((m_length / QTE_MIN) * QTE_MIN_TIME) + randomNumberF(-QTE_RAND_OFFSET, QTE_RAND_OFFSET);
	m_timeToBeActive = (FISHINGSPOT_ACTIVE_TIME * 0.5f) +
		(FISHINGSPOT_ACTIVE_TIME * (m_length / (((QTE_MIN + QTE_MAX) * 0.5f)) * QTE_MIN_TIME)); //stay active for longer if this is a harder combo spot
	cout << "m_length: " << m_length;
	cout << ", m_time: " << m_time;
	cout << ", m_timeToBeActive: " << m_timeToBeActive << endl;
}

void FishingSpot::update(float dt) {
	m_position.y += SCROLL_SPEED * dt;
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);	
	if (m_activeTimer.getElapsedTime().asSeconds() > m_timeToBeActive)
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
