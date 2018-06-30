#pragma once

#include "Wave.h"

class MimicWave : public sf::Drawable {
public:
	MimicWave(const Wave* _wave, const MimicWave* _behindWave, float hMult, float yOffset);
	void start(const sf::Color& topColor, const sf::Color& botColor);
	void update(float dt, float viewBot);
	float getCurrentHeight(int idx) const;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
	const Wave* wave;
	const MimicWave* behindWave;
	sf::VertexArray m_vertices;
	float m_waterLevel;
	float m_viewBot;
	float m_hMult;
	float m_yOffset;
};
