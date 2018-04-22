#pragma once

#include "GameData.h"
#include "Constants.h"

class UIManager : public sf::Drawable {
public:
	UIManager(const sf::RenderWindow* _window);

	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	const sf::RenderWindow* window;

	sf::Text m_scoreDisplay;
	sf::Text m_timerDisplay;

	int m_score;
	sf::Clock m_timer;
};