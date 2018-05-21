#include "GodRay.h"
#include "GameData.h"

GodRay::GodRay() : EASE_DURATION(0) {
}

GodRay::GodRay(std::string spriteName, float position_x, float easeDuration) : EASE_DURATION(easeDuration) {
	m_sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>(spriteName));
	m_position = sf::Vector2f(position_x, 0);
	m_sprite.setPosition(m_position);
}

void GodRay::update(float dt){
	m_timer += dt;
	fadeInOut();
}

void GodRay::fadeInOut() {

	if (m_timer < EASE_DURATION)
		m_alpha = 0.1f + 0.9f * quadraticEaseInOut(m_timer, 0.f, 1.f, EASE_DURATION);
	else
		m_timer = 0;	
	
	m_sprite.setColor(sf::Color(255, 255, 255, 255 * m_alpha));
}

float GodRay::quadraticEaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1)
		return ((c / 2)*(t*t)) + b;

	return 1 - (-c / 2 * (((--t)*(t - 2)) - 1)) + b;
}

void GodRay::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
