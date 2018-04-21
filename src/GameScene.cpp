#include "GameScene.h"
#include "SceneManager.h"
#include "GameData.h"

GameScene::GameScene(sf::RenderWindow* _window)
	: Scene(Type::GameScene, _window) {
	m_view = _window->getView();
	debugCircle.setFillColor(sf::Color::Green);
	debugCircle.setRadius(2);
}

void GameScene::start() {
	m_player.start(sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
}

void GameScene::stop() {

}

void GameScene::handleEvents(const sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return) {
		SceneManager::getInstance().changeScene(Type::GameOverScene);
	}
}

void GameScene::update(float dt) {
	m_player.update(dt);
	debugCircle.setPosition(m_player.getPosition().x, m_player.getPosition().y);
	autoScroll(dt);
}

void GameScene::render(sf::RenderStates states) const {
	m_player.draw(*window, states);
	window->draw(debugCircle);
	window->setView(m_view);
}

void GameScene::autoScroll(float dt) {
	const float SCROLL_SPEED = -.1f * (dt * 1000);
	m_view.move(sf::Vector2f(0.f, SCROLL_SPEED));
}
