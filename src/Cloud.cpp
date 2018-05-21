#include "Cloud.h"
#include "GameData.h"

Cloud::Cloud() {
}

Cloud::Cloud(std::string cloudIndex, bool startCloud) {
	m_alive = true;

	if (cloudIndex == "cloud1" || cloudIndex == "cloud2") {
		m_position.y = SCREEN_HEIGHT * 0.3f;
		m_velocity_x = LARGE_CLOUD_SPEED;
	}
	else if (cloudIndex == "cloud3" || cloudIndex == "cloud4") {
		m_position.y = SCREEN_HEIGHT * 0.5f;
		m_velocity_x = MEDIUM_CLOUD_SPEED;
	}
	else {
		m_position.y = SCREEN_HEIGHT * 0.6f;
		m_velocity_x = SMALL_CLOUD_SPEED;
	}

	m_sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>(cloudIndex));

	if (startCloud)
		m_position.x = rand() % SCREEN_WIDTH;
	else
		m_position.x = SCREEN_WIDTH + m_sprite.getGlobalBounds().width;
	m_position.y -= m_sprite.getGlobalBounds().height;
	m_sprite.setPosition(m_position);
}

void Cloud::update(float dt){
	m_position.x -= m_velocity_x;
	m_sprite.setPosition(m_position);

	if (m_position.x + m_sprite.getGlobalBounds().width < 0)
	{
		m_alive = false;
	}
}

void Cloud::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
