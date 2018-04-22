#include "PlatformManager.h"
#include "Constants.h"

PlatformManager::PlatformManager(const sf::RenderWindow* _window)
	: window(_window) 
	, m_lastY(-1) {
}

int PlatformManager::addStartPlatform(int y, int lastX) {
	//TODO: forgot about Helpers::randomNumber...
	int x;
	//if (lastX == -1) { //this is the first platform on this y value
		x = (rand() % (TILES_X - (int)PLATFORM_SIZE_X + 1)) * TILE_SIZE;
	//}
	//else {
	//	int rightMost = lastX + PLATFORM_SIZE_X;
	//	bool left = true;
	//	bool right = true;
	//	if (lastX < PLATFORM_SIZE_X)
	//		left = false;
	//	else if (lastX >= TILES_X - (2 * PLATFORM_SIZE_X))
	//		right = true;
	//	if (left && right) {
	//		if (rand() % 2)  //left
	//			x = (rand() % (lastX - (int)PLATFORM_SIZE_X + 1)) * TILE_SIZE;
	//		else //right
	//			x = Helpers::randomNumber(rightMost, (TILES_X - (int)PLATFORM_SIZE_X + 1));
	//	}
	//	else if (left) 
	//		x = (rand() % (lastX - (int)PLATFORM_SIZE_X + 1)) * TILE_SIZE;
	//	else if (right)
	//		x = Helpers::randomNumber(rightMost, (TILES_X - (int)PLATFORM_SIZE_X + 1));
	//}
	m_platforms.push_back(Platform(sf::Vector2f(x, y * TILE_SIZE), window));
	return x;
}


void PlatformManager::start() {
	//initial platforms on screen
	m_platforms.clear();
	//set up the platform which the player starts on
	m_platforms.push_back(Platform(sf::Vector2f(PLATFORM_PLAYER_START_X * TILE_SIZE, PLATFORM_PLAYER_START_Y * TILE_SIZE), window));
	m_playerStartPosition = m_platforms.back().getPosition();
	m_playerStartPosition.x += PLATFORM_SIZE_X * 0.5f * TILE_SIZE;
	m_playerStartPosition.y -= TILE_SIZE * 0.05f;

	//random x position but yet y start for platforms
	for (int i = 0; i < PLATFORM_START_Y_SIZE; i++) {
		int y = PLATFORM_START_Y[i];
		int lastX = addStartPlatform(y);
		//if (rand() % 4 == 0) //25% to have two platforms on same y level
		//	addStartPlatform(y, lastX);
	}
}

void PlatformManager::update(float dt) {
	for (int i = 0; i < m_platforms.size(); i++) {
		m_platforms[i].update(dt);
	}
	//remove platforms which are no longer on the screen (alive)
	m_platforms.erase(
		std::remove_if(m_platforms.begin(), m_platforms.end(),
					   [](const Platform& p) {return !p.getAlive(); }),
						m_platforms.end());

	if (m_spawnTimer.getElapsedTime().asSeconds() > PLATFORM_SPAWN_TIME) {
		m_spawnTimer.restart();
		//find top of window
		float top = window->getView().getCenter().y - (SCREEN_HEIGHT * 0.5f);
		int y = ((int)top - ((int)top % TILE_SIZE)) - TILE_SIZE;
		if (y != m_lastY) {//dont spawn on same grid row
			m_lastY = y;
			int x = (rand() % (TILES_X - (int)PLATFORM_SIZE_X + 1)) * TILE_SIZE;
			m_platforms.push_back(Platform(sf::Vector2f(x, y), window));
		}
	}
}
void PlatformManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (int i = 0; i < m_platforms.size(); i++) {
		target.draw(m_platforms[i]);
	}
}

sf::Vector2f PlatformManager::getPlayerStartPosition() const {
	return m_playerStartPosition;
}

const std::vector<Platform>& PlatformManager::getPlatforms() const {
	return m_platforms;
}
