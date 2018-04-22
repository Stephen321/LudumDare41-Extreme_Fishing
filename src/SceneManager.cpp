#include "SceneManager.h"
#include "LoadScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

SceneManager::SceneManager()
	: m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), TITLE)
	, m_pause(false)
	, m_currentScene(nullptr) {
	se::Settings::setErrorFunction(SpriterEngine::Settings::simpleError);
	m_scenes.push_back(new LoadScene(&m_window));
	loadThread = std::thread(&LoadScene::loadData, dynamic_cast<LoadScene*>(m_scenes.back()), std::ref(m_window));
	changeScene(Scene::Type::LoadScene);
	m_clock.restart();
}

SceneManager::~SceneManager() {
	for (int i = 0; i < m_scenes.size(); i++) {
		delete m_scenes[i];
	}
}
void SceneManager::handleEvents() {
	sf::Event ev;
	while (m_window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed ||
			(ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) {
			setRunning(false);
		}
		if (ev.type == sf::Event::LostFocus) {
			m_pause = true;
		}
		if (ev.type == sf::Event::GainedFocus) {
			m_pause = false;
		}
		m_currentScene->handleEvents(ev);
	}
}

void SceneManager::update() {
	if (!getRunning()) {
		loadThread.join();
		return;
	}
	float dt = m_clock.restart().asSeconds();
	//if (m_pause)
	//	dt = 0.f;
	m_currentScene->update(dt);
}

void SceneManager::render() {
	m_window.clear(sf::Color(96, 23, 54));
	m_currentScene->render(sf::RenderStates::Default);
	m_window.display();
}

void SceneManager::changeScene(Scene::Type type) {
	for (int i = 0; i < m_scenes.size(); ++i) {
		if (m_scenes[i]->getType() == type) {
			if (m_currentScene != nullptr)
				m_currentScene->stop();
			m_currentScene = m_scenes[i];
			m_currentScene->start();
			m_clock.restart();
			return;
		}
	}
	assert(false && "No scene of that type could be found.");
}

void SceneManager::finishedLoading() {
	m_scenes.push_back(new MenuScene(&m_window));
	m_scenes.push_back(new GameScene(&m_window));
	m_scenes.push_back(new GameOverScene(&m_window));
	//todo this should go to menuscene
	changeScene(Scene::Type::GameScene);
}

bool SceneManager::getPaused() const {
	return m_pause;
}
