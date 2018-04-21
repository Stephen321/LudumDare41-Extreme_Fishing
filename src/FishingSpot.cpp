#include "FishingSpot.h"
#include "GameData.h"
#include "Constants.h"
#include "Helpers.h"

using namespace Helpers;

FishingSpot::FishingSpot(){
}

void FishingSpot::start(const sf::Vector2f start) {
	m_position = start;
	m_entity = GameData::getInstance().getAsset<se::EntityInstance*>("FishingSpot");
	m_entity->setCurrentAnimation("NewAnimation");

	m_entity->setScale(se::point(0.5f, 0.5f));
	m_entity->setPosition(se::vectorToPoint(start));
}

void FishingSpot::update(float dt) {
	
}

void FishingSpot::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	m_entity->render();
}
