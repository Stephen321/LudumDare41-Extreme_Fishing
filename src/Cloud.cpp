#include "Cloud.h"
#include "GameData.h"

Cloud::Cloud() {
	m_alive = true;
	float chance = rand() % 100;

	if (chance > 90){		
		while (m_previousIndex == m_currentIndex) {
			m_currentIndex = ceil(rand() % 2) + 1;
		}
		m_position.y = SCREEN_HEIGHT * 0.3f;
		m_velocity_x = LARGE_CLOUD_SPEED;
	}
	else if (chance > 60) {
		while (m_previousIndex == m_currentIndex) {
			m_currentIndex = ceil(rand() % 2) + 3;
		}
		m_position.y = SCREEN_HEIGHT * 0.5f;
		m_velocity_x = MEDIUM_CLOUD_SPEED;
	}
	else {
		while (m_previousIndex == m_currentIndex) {
			m_currentIndex = ceil(rand() % 4) + 5;
		}
		m_position.y = SCREEN_HEIGHT * 0.6f;
		m_velocity_x = SMALL_CLOUD_SPEED;
	}

	m_previousIndex = m_currentIndex;
	std::string cloudIndex = "cloud" + std::to_string(m_currentIndex);
	m_sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>(cloudIndex));
	m_position.x = -m_sprite.getGlobalBounds().width;
	m_position.y -= m_sprite.getGlobalBounds().height;
	m_sprite.setPosition(m_position);
}

Cloud::Cloud(std::string cloudIndex) {
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
	m_position.x = rand() % SCREEN_WIDTH;
	m_position.y -= m_sprite.getGlobalBounds().height;
	m_sprite.setPosition(m_position);
}

Cloud::~Cloud() {
}

void Cloud::update(float dt){
	m_position.x += m_velocity_x;
	m_sprite.setPosition(m_position);

	if (m_position.x > SCREEN_WIDTH)
	{
		m_alive = false;
	}
}

void Cloud::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
