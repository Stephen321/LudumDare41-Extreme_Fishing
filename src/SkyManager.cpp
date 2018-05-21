#include "SkyManager.h"

SkyManager::SkyManager() {

}

void SkyManager::start() {
		m_clouds.push_back(Cloud("cloud2", true));
		m_clouds.push_back(Cloud("cloud5", true));
		m_clouds.push_back(Cloud("cloud8", true));
		m_timer = CLOUD_SPAWN_DELAY;
		m_godRays.push_back(GodRay("godRay1", SCREEN_WIDTH * 0.33f, 5.f));
		m_godRays.push_back(GodRay("godRay2", SCREEN_WIDTH * 0.01f, 3.f));
		m_godRays.push_back(GodRay("godRay3", SCREEN_WIDTH * 0.5f, 6.f));
		m_godRays.push_back(GodRay("godRay4", SCREEN_WIDTH * 0.9f, 4.f));
}

void SkyManager::update(float dt) {

	m_timer += dt;
	spawnCloud();

	for (int i = 0; i < m_clouds.size(); i++) {
		m_clouds[i].update(dt);

		if (m_clouds[i].getAlive() == false) {
			m_clouds.erase(m_clouds.begin() + i);
		}
	}

	for (int i = 0; i < m_godRays.size(); i++) {
		m_godRays[i].update(dt);
	}
}

void SkyManager::spawnCloud() {

	if (m_timer > CLOUD_SPAWN_DELAY) {
		float probability = rand() % 100;

		if (probability > LARGE_CLOUD_PROBILITY) {
			while (m_previousCloudIndex == m_currentCloudIndex) {
				m_currentCloudIndex = ceil(rand() % 2) + 1;
			}
		}
		else if (probability > MEDIUM_CLOUD_PROBILITY) {
			while (m_previousCloudIndex == m_currentCloudIndex) {
				m_currentCloudIndex = ceil(rand() % 2) + 3;
			}
		}
		else {
			while (m_previousCloudIndex == m_currentCloudIndex) {
				m_currentCloudIndex = ceil(rand() % 4) + 5;
			}
		}

		m_previousCloudIndex = m_currentCloudIndex;		
		m_clouds.push_back(Cloud(std::string("cloud" + std::to_string(m_currentCloudIndex)), false));
		m_timer = 0;
	}
}

void SkyManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (int i = 0; i < m_godRays.size(); i++) {
		target.draw(m_godRays[i]);
	}

	for (int i = 0; i < m_clouds.size(); i++) {
		target.draw(m_clouds[i]);
	}
}