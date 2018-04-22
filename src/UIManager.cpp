#include "..\include\UIManager.h"

UIManager::UIManager(const sf::RenderWindow * _window)
	: window(_window) {
}

void UIManager::start(){
	m_timer.restart();

	m_scoreDisplay.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_scoreDisplay.setPosition(0.9 * SCREEN_WIDTH, 0.02 * SCREEN_HEIGHT);
	m_scoreDisplay.setScale(sf::Vector2f(2.f, 2.f));

	m_timerDisplay.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	sf::FloatRect textRect = m_timerDisplay.getLocalBounds();
	m_timerDisplay.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_timerDisplay.setPosition(sf::Vector2f(0.5 * SCREEN_WIDTH, 0.02 * SCREEN_HEIGHT));
	m_timerDisplay.setScale(sf::Vector2f(3.f, 3.f));
}

void UIManager::update(float dt){
	m_scoreDisplay.setString(std::to_string(m_score));
	m_timerDisplay.setString(std::to_string((int)m_timer.getElapsedTime().asSeconds()));
}

void UIManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_scoreDisplay);
	target.draw(m_timerDisplay);
}
