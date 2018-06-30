#pragma once

#include "MimicWave.h"
#include "Constants.h"
#include "Helpers.h"

MimicWave::MimicWave(const Wave* _wave, const MimicWave* _behindWave, float hMult, float yOffset)
	: wave(_wave)
	, behindWave(_behindWave)
	, m_hMult(hMult)
	, m_yOffset(yOffset) {
	m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
}

void MimicWave::start(const sf::Color& topColor, const sf::Color& botColor) {
	//IMPORTANT: TODO: these colours are interpolated all the way to the bottom of the screen...
	//change the bottom y position of this to always match the top y of the layer in front of it...
	m_vertices = wave->getVertices();
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		m_vertices[i * 2].color = botColor;
		m_vertices[(i * 2) + 1].color = topColor;
	}
}

void MimicWave::update(float dt, float viewBot) {
	m_waterLevel = viewBot - WATER_HEIGHT; //TODO: this and the one in Wave have to be the same 
	m_viewBot = viewBot;

	//update drawable vertices
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		float frontCurrentHeight = wave->getCurrentHeight(i);
		float diff = -(WATER_HEIGHT - frontCurrentHeight) * m_hMult;
		float currentHeight = WATER_HEIGHT + diff;

		float behindCurrentHeight;
		if (behindWave != nullptr) {
			behindCurrentHeight = behindWave->getCurrentHeight(i);
		}
		else {
			behindCurrentHeight = wave->getCurrentHeight(i);
		}

		m_vertices[i * 2].position.y = m_viewBot - (currentHeight + m_yOffset);
		m_vertices[(i * 2) + 1].position.y = m_viewBot - behindCurrentHeight;
	}
}


float MimicWave::getCurrentHeight(int idx) const {
	if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
		return 0.f;
	return m_viewBot - m_vertices[idx * 2].position.y;
}

void MimicWave::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(m_vertices);
}