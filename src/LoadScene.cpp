#include "LoadScene.h"
#include "SceneManager.h"
#include "Constants.h"
#include "GameData.h"

LoadScene::LoadScene(sf::RenderWindow* _window)
	: Scene(Type::LoadScene, _window) {
	m_load = new se::SpriterModel("assets/animations/Loading/loading.scon", new se::ExampleFileFactory(window), new se::ExampleObjectFactory(window));
	m_background = m_load->getNewEntityInstance("LoadingScreen");
	m_background->setCurrentAnimation("Idle");
	m_background->setPosition(se::point(0.f, 0.f));
	setFinished(false);
}

LoadScene::~LoadScene() {
	delete m_load;
}

void LoadScene::loadData(sf::RenderWindow& window) {
	GameData::getInstance().load(&window);
	setFinished(true);
}

void LoadScene::setFinished(bool _finished) {
	mutex.lock();
	finished = _finished;
	mutex.unlock();
}

bool LoadScene::getFinished() {
	bool f;
	mutex.lock();
	f = finished;
	mutex.unlock();
	return f;
}

void LoadScene::start() {
	finished = false;
	window->setActive(false);
}

void LoadScene::stop() {

}

void LoadScene::handleEvents(const sf::Event& ev) {
}

void LoadScene::update(float dt) {
	if (getFinished())
		SceneManager::getInstance().finishedLoading();
	m_background->setTimeElapsed(dt);
}

void LoadScene::render(sf::RenderStates states) const {
	m_background->render();
}