#include "GameScene.h"
#include "SceneManager.h"
#include "GameData.h"

GameScene::GameScene(sf::RenderWindow* _window)
	: Scene(Type::GameScene, _window)
	, m_platformManager(_window) {
	m_view = _window->getView();
	debugCircle.setFillColor(sf::Color::Green);
	debugCircle.setRadius(2);
}

void GameScene::start() {
	m_platformManager.start(); // call this first so it sets player start position
	m_player.start(m_platformManager.getPlayerStartPosition());
}

void GameScene::stop() {

}

void GameScene::handleEvents(const sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return) {
		SceneManager::getInstance().changeScene(Type::GameOverScene);
	}
}

void GameScene::update(float dt) {
	m_platformManager.update(dt);
	m_player.update(dt);
	m_player.checkCollisions(m_platformManager.getPlatforms());
	debugCircle.setPosition(m_player.getPosition().x, m_player.getPosition().y);
	//autoScroll(dt);
}

void GameScene::render(sf::RenderStates states) const{
	window->draw(m_player);
	window->draw(m_platformManager);
	window->draw(debugCircle);


	sf::RectangleShape d;
	d.setFillColor(sf::Color(0,255,0,100));
	d.setPosition(m_player.getBoundingBox().left, m_player.getBoundingBox().top);
	d.setSize(sf::Vector2f(m_player.getBoundingBox().width, m_player.getBoundingBox().height));

	window->draw(d);
	for (int i = 0; i < m_platformManager.getPlatforms().size(); i++) {
		sf::IntRect bb = m_platformManager.getPlatforms()[i].getBoundingBox();
		d.setPosition(bb.left, bb.top);
		d.setSize(sf::Vector2f(bb.width, bb.height));
		window->draw(d);
	}
}

void GameScene::autoScroll(float dt) {
	const float SCROLL_SPEED = -.1f * (dt * 1000);
	m_view.move(sf::Vector2f(0.f, SCROLL_SPEED));
	window->setView(m_view);
}
