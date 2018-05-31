#include "Island.h"
#include "GameData.h"

Island::Island() {
	m_island_sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>("island"));
	m_island_sprite.setPosition(sf::Vector2f(0, SCREEN_HEIGHT - m_island_sprite.getGlobalBounds().height));

	m_position = sf::Vector2f(700, 800);
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("Palmtree")->getNewEntityInstance("Palmtree");
	se::changeAnimation(m_entity, PALMTREE_IDLE_ANIM);
	m_entity->setPosition(se::vectorToPoint(m_position));
}

void Island::update(float dt) {
	m_entity->setTimeElapsed(dt);
}

void Island::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	m_entity->render();
	target.draw(m_island_sprite);
}
