#pragma once

#include "Platform.h"

class PlatformManager : public sf::Drawable {
public:
	PlatformManager(const sf::RenderWindow* _window);
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	sf::Vector2f getPlayerStartPosition() const;
	const std::vector<Platform>& getPlatforms() const;
private:
	int addStartPlatform(int y, int lastX = -1);
	std::vector<Platform> m_platforms;
	sf::Clock m_spawnTimer;
	const sf::RenderWindow* window;
	int m_lastY;
	sf::Vector2f m_playerStartPosition;
};
