#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Cloud.h"
#include "GodRay.h"

class SkyManager : public sf::Drawable {
public:
	SkyManager();
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void spawnCloud();
private:
	std::vector<Cloud> m_clouds;
	int m_currentCloudIndex = 0;
	int m_previousCloudIndex = 0;
	std::vector<GodRay> m_godRays;
	float m_timer = 0;
};