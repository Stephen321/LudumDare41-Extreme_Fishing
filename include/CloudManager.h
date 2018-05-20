#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Cloud.h"

class CloudManager : public sf::Drawable {
public:
	CloudManager();
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void spawnCloud();
private:
	std::vector<Cloud> m_clouds;
	float m_time = 0;
};