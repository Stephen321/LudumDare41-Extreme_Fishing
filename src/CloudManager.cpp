#include "CloudManager.h"

CloudManager::CloudManager() {

}

void CloudManager::start() {
		m_clouds.push_back(Cloud("cloud2"));
		m_clouds.push_back(Cloud("cloud5"));
		m_clouds.push_back(Cloud("cloud8"));
		m_clouds.push_back(Cloud());
}

void CloudManager::update(float dt) {

	m_time += dt;
	spawnCloud();

	for (int i = 0; i < m_clouds.size(); i++) {
		m_clouds[i].update(dt);

		if (m_clouds[i].getAlive() == false) {
			m_clouds.erase(m_clouds.begin() + i);
		}
	}
}

void CloudManager::spawnCloud() {
	if (m_time > 5.f) {
		m_clouds.push_back(Cloud());
		m_time = 0;
	}
}

void CloudManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (int i = 0; i < m_clouds.size(); i++) {
		target.draw(m_clouds[i]);
	}
}