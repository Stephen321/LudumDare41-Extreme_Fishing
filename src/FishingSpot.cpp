#include "FishingSpot.h"
#include "GameData.h"
#include "Constants.h"
#include "Helpers.h"

using namespace Helpers;

FishingSpot::FishingSpot(){
	m_alive = false;
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("fishingSpot")->getNewEntityInstance("FishingSpot");
}

void FishingSpot::start(const sf::Vector2f start, int x, float timeToBeActive) {
	m_position = start;
	m_x = x;
	m_timeToBeActive = timeToBeActive;
	m_entity->setCurrentAnimation("Bubbles");
	m_activeTimer.restart();
	m_alive = true;
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
