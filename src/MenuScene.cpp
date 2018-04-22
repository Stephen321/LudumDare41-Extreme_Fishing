#include "MenuScene.h"
#include "GameData.h"
#include "SceneManager.h"

MenuScene::MenuScene(sf::RenderWindow* _window)
	: Scene(Type::MenuScene, _window) {
	sf::View view = window->getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	m_background.setTexture(GameData::getInstance().getAsset<sf::Texture>("menu"));
	m_background.setPosition(0.f, 0.f);
	m_background.setScale(bounds.width / m_background.getLocalBounds().width, bounds.height / m_background.getLocalBounds().height);

	m_title.setTexture(GameData::getInstance().getAsset<sf::Texture>("title"));
	m_title.setOrigin(m_title.getTexture()->getSize().x / 2, m_title.getTexture()->getSize().y / 2);
	m_title.setPosition(window->getSize().x / 2, 0.2 * window->getSize().y);

	m_playLogo.setTexture(GameData::getInstance().getAsset<sf::Texture>("play"));
	m_playLogo.setOrigin(m_playLogo.getTexture()->getSize().x / 2, m_playLogo.getTexture()->getSize().y / 2);
	m_playLogo.setPosition(window->getSize().x / 2, 0.8 * window->getSize().y);
	
	m_fade = false;
}

void MenuScene::start() {
	m_fadeTimer.restart();
}

void MenuScene::stop() {

}

void MenuScene::handleEvents(const sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Space) {
		SceneManager::getInstance().changeScene(Type::GameScene);
	}
}

void MenuScene::update(float dt) {
	if (m_playLogo.getColor().a == 255) {
		m_fade = true;
	}
	else if (m_playLogo.getColor().a == 0) {
		m_fade = false;
	}
	
	if (m_fade) {
		m_playLogo.setColor(sf::Color(m_playLogo.getColor().r, m_playLogo.getColor().g, m_playLogo.getColor().b, m_playLogo.getColor().a - 1));
	}
	else {
		m_playLogo.setColor(sf::Color(m_playLogo.getColor().r, m_playLogo.getColor().g, m_playLogo.getColor().b, m_playLogo.getColor().a + 1));
	}
}

void MenuScene::render(sf::RenderStates states) const {
	window->draw(m_background);
	window->draw(m_title);
	window->draw(m_playLogo);
}

void MenuScene::fadeOut(float dt, sf::Sprite logo)
{
	/*if (m_playLogo.getColor().a >= 255) {
		m_playLogo.setColor(sf::Color(0,0,0,))
	}*/
}
