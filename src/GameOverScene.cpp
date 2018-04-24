#include "GameOverScene.h"
#include "GameData.h"
#include "SceneManager.h"

GameOverScene::GameOverScene(sf::RenderWindow* _window)
	: Scene(Type::GameOverScene, _window) {
	sf::View view = window->getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	m_background.setTexture(GameData::getInstance().getAsset<sf::Texture>("gameover"));
	m_background.setPosition(0.f, 0.f);
	m_background.setScale(bounds.width / m_background.getLocalBounds().width, bounds.height / m_background.getLocalBounds().height);


	m_scoreDisplay.setFont(GameData::getInstance().getAsset<sf::Font>("font"));
	m_scoreDisplay.setCharacterSize(UI_FONT_SIZE * 4);
	m_scoreDisplay.setPosition(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.4f);
}

void GameOverScene::start() {
	m_scoreDisplay.setString(std::string(UI_SCORE_PREFIX) + std::to_string(GameData::getInstance().score) +
	std::string("\n Press Space to try again."));
}

void GameOverScene::stop() {

}

void GameOverScene::handleEvents(const sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Space) {
		SceneManager::getInstance().changeScene(Type::GameScene);
	}
}

void GameOverScene::update(float dt) {
}

void GameOverScene::render(sf::RenderStates states) const {
	window->draw(m_background);
	window->draw(m_scoreDisplay);
}