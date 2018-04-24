#include "UIManager.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "GameData.h"

UIManager::UIManager(const sf::RenderWindow * _window, const Player* _player)
	: window(_window)
	, player(_player) {
}

void UIManager::start(){
	m_timer = 0.f;

	m_scoreDisplay.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_timerDisplay.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_qteTimeRemaining.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_lives.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_scoreDisplay.setCharacterSize(UI_FONT_SIZE);
	m_timerDisplay.setCharacterSize(UI_FONT_SIZE);
	m_qteTimeRemaining.setCharacterSize(UI_FONT_SIZE);
	m_lives.setCharacterSize(UI_FONT_SIZE);

	//sf::FloatRect textRect = m_timerDisplay.getLocalBounds();
	//m_timerDisplay.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

void UIManager::update(float dt){
	m_timer += dt;

	m_scoreDisplay.setString(std::string(UI_SCORE_PREFIX) + std::to_string(GameData::getInstance().score));
	m_timerDisplay.setString(std::string(UI_TIMER_PREFIX) + std::to_string((int)m_timer));
	m_lives.setString(std::string("Lives: ") + std::to_string(GameData::getInstance().lives));
	float t = player->getQteTime();
	if (t < 0.f)
		t = 0.f;
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << t;
	std::string s = stream.str();
	m_qteTimeRemaining.setString(s + std::string(UI_QTE_TIMER_SUFFIX));

	float top = window->getView().getCenter().y - (SCREEN_HEIGHT * 0.5f);
	m_lives.setPosition(UI_X, top + UI_Y);
	m_scoreDisplay.setPosition(UI_X, top + (UI_Y * 2.f));
	m_timerDisplay.setPosition(sf::Vector2f(UI_X, top + (UI_Y * 4.f)));
	m_qteTimeRemaining.setPosition(player->getPosition() + sf::Vector2f(UI_SCORE_X_OFFS, UI_SCORE_Y_OFFS));

}

void UIManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_scoreDisplay);
	target.draw(m_timerDisplay);
	target.draw(m_lives);
	if (*player->getQte())
		target.draw(m_qteTimeRemaining);

}