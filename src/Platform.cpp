#include "Platform.h"
#include "GameData.h"
#include "Constants.h"

Platform::Platform(const sf::Vector2f& start, const sf::RenderTarget* _window)
	: window(_window) {
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("platform")->getNewEntityInstance("Platform");
	
	m_position = start;
	m_entity->setCurrentAnimation("Idle");
	m_entity->setPosition(se::vectorToPoint(start));
}

void Platform::update(float dt) {
	if (!m_alive)
		return;
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);
	if (m_position.y > window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f))
		m_alive = false;
}

void Platform::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (!m_alive)
		return;
	m_entity->render();
}

sf::IntRect Platform::getBoundingBox() const {
	sf::IntRect bb;
	bb.left = (int)m_position.x;
	bb.top = (int)m_position.y;
	bb.width = PLATFORM_SIZE_X * TILE_SIZE;
	bb.height = (int)(PLATFORM_SIZE_Y * TILE_SIZE);
	return bb;
}
