#pragma once

#include "Wave.h"
#include "Helpers.h"

Wave::Wave() {
	m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
}
void Wave::start(float viewBot) {
	createSines();
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		Spring s;
		s.position.x = ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT) * i;
		s.position.y = WATER_HEIGHT;
		m_springs[i] = s;
	}
	m_vertices.clear();

	sf::Vertex v1, v2;
	v1.color = WATER_TOP_COLOR;
	v2.color = WATER_BOT_COLOR;
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		float x = m_springs[i].position.x;
		float y = m_springs[i].position.y;
		v1.position = sf::Vector2f(x, viewBot - y);
		v2.position = sf::Vector2f(x, viewBot);
		m_vertices.append(v1);
		m_vertices.append(v2);
	}
}
void Wave::update(float dt, float viewBot) {
	m_waterLevel = viewBot - WATER_HEIGHT;// -(BACKGROUND_WAVE_MAX_HEIGHT * 0.5f);// todo cant do this cause randomly decided on the wave height..
	m_viewBot = viewBot;
	m_offset++;

	//------------------------------------------------------------------------------
	//update spring physics
	const int WATER_PROP_LOOPS = 10;
	const float SPREAD = 0.45;
	sf::Vector2f deltas[WATER_SPRINGS_COUNT];
	for (int i = 0; i < WATER_PROP_LOOPS; i++) {
		for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
			if (i > 0) {
				deltas[i].x = SPREAD * (m_springs[i].position.y - m_springs[i - 1].position.y);
				m_springs[i - 1].velocity.y += deltas[i].x;
			}
			if (i < WATER_SPRINGS_COUNT - 1) {
				deltas[i].y = SPREAD * (m_springs[i].position.y - m_springs[i + 1].position.y);
				m_springs[i + 1].velocity.y += deltas[i].y;
			}
		}
	}
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		if (i > 0)
			m_springs[i - 1].position.y += deltas[i].x;
		if (i < WATER_SPRINGS_COUNT - 1)
			m_springs[i + 1].position.y += deltas[i].y;
	}
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		float x = WATER_HEIGHT - m_springs[i].position.y;
		m_springs[i].acceleration = sf::Vector2f(0.f, WATER_SPRING_K * x);
		m_springs[i].velocity += m_springs[i].acceleration;
		m_springs[i].velocity -= m_springs[i].velocity * WATER_SPRING_DAMPENER;
		if (Helpers::getLength(m_springs[i].velocity) < 0.01f)
			m_springs[i].velocity.x = m_springs[i].velocity.y = 0.f;
		m_springs[i].position += m_springs[i].velocity * dt;
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	//update drawable vertices
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		m_vertices[i * 2].position.y = m_viewBot - getCurrentHeight(i);
		m_vertices[(i * 2) + 1].position.y = m_viewBot;
	}
	//------------------------------------------------------------------------------
}
void Wave::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(m_vertices);

	sf::CircleShape s(4);
	s.setFillColor(sf::Color(0, 0, 255, 160));
	sf::CircleShape s2(2);
	s2.setFillColor(sf::Color(255, 0, 0, 160));
	s2.setOrigin(1, 1);
	for (int i = 1; i < WATER_SPRINGS_COUNT - 1; i++) {
		sf::Vector2f pos = sf::Vector2f(m_springs[i].position.x, m_viewBot - getCurrentHeight(i));
		s.setPosition(pos);
		pos.y = m_waterLevel;
		//pos.y = m_viewBot - spring;//todo: this looks so much better...
		s2.setPosition(pos);
		//target.draw(s);
		//target.draw(s2);
	}

	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		sf::Vector2f pos = m_vertices[i * 2].position;
		s.setFillColor(sf::Color(255, 10, 50, 160));
		s.setPosition(pos);
		//target.draw(s);
	}
}

float Wave::getWave(int idx) const {
	//returns the wave at the current idx
	if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
		return 0.f;
	return (overlapSines(m_springs[idx].position.x));
}

float Wave::getSpringY(int idx) const {
	if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
		return 0.f;
	return m_springs[idx].position.y;
}

float Wave::getCurrentHeight(int idx) const {
	if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
		return 0.f;
	float wave = getWave(idx); //wave y
	float spring = getSpringY(idx);//spring physics y
	return spring + wave; //current height of the wave
}

sf::Vector2f Wave::splash(float xPos, float strength) {
	//todo: move velocity calculation to the same place as position calculation...
	//todo refactor this
	int x = xPos / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
	float wave = (overlapSines(m_springs[x].position.x)); //get what the wave is at this point
	sf::Vector2f start;
	start.x = m_springs[x].position.x + (SPLASH_PARTICLES_X_MAXOFFSET * Helpers::randomNumberF(-1.f, 1.f));
	start.y = m_viewBot - m_springs[x].position.y - wave;

	m_springs[x].position.y = WATER_HEIGHT - (SPLASH_PARTICLES_WAVE_MIN_STRENGTH + (strength * SPLASH_PARTICLES_WAVE_STRENGTH));

	return start;
}

sf::VertexArray Wave::getVertices() const {
	return m_vertices;
}

void Wave::createSines() {
	//https://gamedev.stackexchange.com/questions/44547/how-do-i-create-2d-water-with-dynamic-waves
	m_offset = 0.f;
	for (int i = -0; i < NUM_BACKGROUND_WAVES; i++) {
		float sineOffset = -PI + (2 * PI * (rand() / (float)RAND_MAX));
		m_sineOffsets.push_back(sineOffset);
		float sineAmplitude = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_MAX_HEIGHT;
		m_sineAmplitudes.push_back(sineAmplitude);
		float sineStretch = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_COMPRESSION;
		m_sineStretches.push_back(sineStretch);
		float offsetStretch = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_COMPRESSION;
		m_offsetStretches.push_back(offsetStretch);
	}
}
float Wave::overlapSines(float x) const {
	float result = 0;
	for (int i = 0; i < NUM_BACKGROUND_WAVES; i++) {
		result += m_sineOffsets[i]
			+ m_sineAmplitudes[i]
			* sin(x * m_sineStretches[i] + m_offset * m_offsetStretches[i]);
	}
	return result;
}