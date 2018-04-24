#pragma once

#include "GameData.h"
#include "Constants.h"
#include "Player.h"

class UIManager : public sf::Drawable {
public:
	UIManager(const sf::RenderWindow* _window, const Player* _playerPos);

	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	const sf::RenderWindow* window;
	sf::Text m_qteTimeRemaining;
	sf::Text m_scoreDisplay;
	sf::Text m_timerDisplay;
	sf::Text m_lives;
	const Player* player;
	float m_timer;
};