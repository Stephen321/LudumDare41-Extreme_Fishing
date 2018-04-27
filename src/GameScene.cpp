#include "GameScene.h"
#include "SceneManager.h"
#include "GameData.h"

GameScene::GameScene(sf::RenderWindow* _window)
	: Scene(Type::GameScene, _window)
	, m_platformManager(_window)
	, m_fishManager(_window, &m_player)
	, m_uiManager(_window, &m_player)
	, m_water(_window) {
	m_view = _window->getView();
	debugCircle.setFillColor(sf::Color::Green);
	debugCircle.setRadius(2);
	m_background.setTexture(GameData::getInstance().getAsset<sf::Texture>("sky"));
	m_background.setScale(SCREEN_WIDTH, 1.f);
	music.openFromFile("assets/audio/music/music.wav");
	music.setLoop(true);
	music.setVolume(100);
}

void GameScene::start() {
	m_platformManager.start(); // call this first so it sets player start position
	m_fishManager.start();
	m_uiManager.start();
	m_water.start();
	m_player.start(m_platformManager.getPlayerStartPosition());
	m_view = window->getDefaultView();
	window->setView(m_view);
	GameData::getInstance().lives = 5;
	GameData::getInstance().score = 0;
	if (!GOD_MODE)
		music.play();
}

void GameScene::stop() {
	music.stop();
	m_view = window->getDefaultView();
	window->setView(m_view);
}

void GameScene::handleEvents(const sf::Event& ev) {
	if (GOD_MODE) {
		if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return) {
			SceneManager::getInstance().changeScene(Type::GameOverScene);
		}
	}
	m_player.handleEvents(ev);
}

void GameScene::update(float dt) {
	if (dt > 1.f || SceneManager::getInstance().getPaused())
		return;

	m_platformManager.update(dt);
	m_fishManager.update(dt);
	m_player.checkCollisions(m_platformManager.getPlatforms());
	m_player.update(dt);
	m_uiManager.update(dt);
	m_water.update(dt);
	m_background.setPosition(m_view.getCenter().x - (SCREEN_WIDTH * 0.5f), m_view.getCenter().y - (SCREEN_HEIGHT * 0.5f));


	if (m_player.getPosition().y > m_water.getLevel() + (TILE_SIZE * WATER_TILES)) {
		SceneManager::getInstance().changeScene(Scene::Type::GameOverScene);
	}
	if (m_player.getAttemptingToFish()) {
		m_fishManager.attempt(&m_player);
	}
	else if (*m_player.getQte()) {
		m_player.setQteFishSpot(m_fishManager.getQteFishSpot());
	}
	//debugCircle.setPosition(m_player.getPosition().x, m_player.getPosition().y);

	//GameData::getInstance().top = window->getView().getCenter().y - (SCREEN_HEIGHT * 0.5f);
	autoScroll(dt);
}

void GameScene::render(sf::RenderStates states) const{
	window->draw(m_background);
	window->draw(m_platformManager);
	window->draw(m_player);
	window->draw(m_water);
	window->draw(m_fishManager);
	window->draw(m_uiManager);
}

void GameScene::autoScroll(float dt) {
	m_view.move(sf::Vector2f(0.f, SCROLL_SPEED * dt));
	window->setView(m_view);
}
