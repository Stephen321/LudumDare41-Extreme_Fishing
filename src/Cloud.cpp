#include "Cloud.h"
#include "GameData.h"

Cloud::Cloud() {
	m_alive = true;

	int index = 0;
	float chance = rand() % 100;

	if (chance > 80){
		index = ceil(rand() % 2) + 1;
		m_position.y = SCREEN_HEIGHT * 0.3f;
		m_velocity_x = 0.7;
	}
	else if (chance > 50) {
		index = ceil(rand() % 2) + 3;
		m_position.y = SCREEN_HEIGHT * 0.5f;
		m_velocity_x = 0.6;
	}
	else {
		index = ceil(rand() % 4) + 5;
		m_position.y = SCREEN_HEIGHT * 0.6f;
		m_velocity_x = 0.5;
	}

	std::string cloudIndex = "cloud" + std::to_string(index);
	m_sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>(cloudIndex));
	m_position.x = -m_sprite.getGlobalBounds().width;
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
