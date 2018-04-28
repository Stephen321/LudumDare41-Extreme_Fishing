#include "Water.h"
#include "GameData.h"
#include "Helpers.h"
#define _USE_MATH_DEFINES 
#include <cmath>

Water::Water(const sf::RenderWindow* _window)
	: window(_window) { 
	//m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("water")->getNewEntityInstance("Water");
	//m_entity->setCurrentAnimation("Idle");
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
}

void Water::start() {
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		Spring s;
		s.position.x = ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT) * i;
		s.position.y = WATER_HEIGHT;
		m_springs[i] = s;
	}
	m_timer.restart();
}

//////////////////////////////////////////////////////////////////
// A phase difference to apply to each sine
float offset = 0;

float NUM_BACKGROUND_WAVES = 7;
float BACKGROUND_WAVE_MAX_HEIGHT = 6;
float BACKGROUND_WAVE_COMPRESSION = 1 / 10;
// Amounts by which a particular sine is offset
std::vector<float> sineOffsets;
// Amounts by which a particular sine is amplified
std::vector<float> sineAmplitudes;
// Amounts by which a particular sine is stretched
std::vector<float> sineStretches;
// Amounts by which a particular sine's offset is multiplied
std::vector<float> offsetStretches;
// Set each sine's values to a reasonable random value

void Water::createSines() {
	const float M_PI = 3.14; //temp
	for (int i = -0; i < NUM_BACKGROUND_WAVES; i++) {
		float sineOffset = -M_PI + 2 * M_PI * (rand() / (float)RAND_MAX);
		sineOffsets.push_back(sineOffset);
		float sineAmplitude = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_MAX_HEIGHT;
		sineAmplitudes.push_back(sineAmplitude);
		float sineStretch = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_COMPRESSION;
		sineStretches.push_back(sineStretch);
		float offsetStretch = (rand() / (float)RAND_MAX) * BACKGROUND_WAVE_COMPRESSION;
		offsetStretches.push_back(offsetStretch);
	}
}
// This function sums together the sines generated above,
// given an input value x
float Water::overlapSines(float x) {
	float result = 0;
	for (int i = 0; i < NUM_BACKGROUND_WAVES; i++) {
		result = result
			+ sineOffsets[i]
			+ sineAmplitudes[i]
			* sin(x * sineStretches[i] + offset * offsetStretches[i]);
	}
	return result;
}
//////////////////////////////////////////////////////////////////

void Water::update(float dt) {
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	//m_entity->setPosition(se::point(0.f, m_level));
	//m_entity->setTimeElapsed(dt);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mouse = sf::Mouse::getPosition();
		splash(mouse.x, mouse.y / 100);
	}

	//waves
	float waveMult = sin(m_timer.getElapsedTime().asSeconds());
	const int WAVE_STRENGTH = 60;
	splash(0, waveMult * WAVE_STRENGTH);

	const int WATER_PROP_LOOPS = 10;
	const float SPREAD = 0.45;
	sf::Vector2f deltas[WATER_SPRINGS_COUNT];
	for (int i = 0; i < WATER_PROP_LOOPS; i++) {
		for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
			if (i > 0) {
				//height diff from left
				deltas[i].x = SPREAD * (m_springs[i].position.y - m_springs[i - 1].position.y);
				m_springs[i - 1].velocity.y += deltas[i].x; //
			}
			if (i < WATER_SPRINGS_COUNT - 1) {
				//height diff from right
				deltas[i].y = SPREAD * (m_springs[i].position.y - m_springs[i + 1].position.y);
				m_springs[i + 1].velocity.y += deltas[i].y;
			}
		}
	}

	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		if (i > 0)
			m_springs[i - 1].position.y += deltas[i].x;
			//m_springs[i - 1].acceleration.y = WATER_SPRING_K * deltas[i].x;
		if (i < WATER_SPRINGS_COUNT - 1)
			m_springs[i + 1].position.y += deltas[i].y;
			//m_springs[i - 1].acceleration.y = WATER_SPRING_K * deltas[i].x;
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
}

void Water::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	//m_entity->render();

	sf::CircleShape s(5);
	s.setOrigin(2.5, 2.5);
	s.setFillColor(sf::Color(0, 0, 255, 128));
	m_vertices.clear();
	sf::Vertex v1, v2;
	sf::Vector2f pos = sf::Vector2f(m_springs[0].position.x, (m_level + WATER_HEIGHT) - m_springs[0].position.y);
	v1.position = pos;
	v1.color = sf::Color(0, 0, 128, 128);

	v2.position = pos + sf::Vector2f(0.f, m_springs[0].position.y);
	v2.color = sf::Color(0, 0, 255, 128);
	m_vertices.append(v1);
	m_vertices.append(v2);

	for (int i = 1; i < WATER_SPRINGS_COUNT - 1; i++) {
		sf::Vector2f pos = sf::Vector2f(m_springs[i].position.x, (m_level + WATER_HEIGHT) - m_springs[i].position.y);
		s.setPosition(pos);
		sf::Vertex v1, v2;
		v1.position = pos;
		v1.color = sf::Color(0, 0, 128, 128);

		v2.position = pos + sf::Vector2f(0.f, m_springs[i].position.y);
		v2.color = sf::Color(0, 0, 255, 128);

		m_vertices.append(v1);
		m_vertices.append(v2);
		target.draw(s);
	}
	target.draw(m_vertices);
}

int Water::getLevel() const {
	return m_level;
}

void Water::splash(float position, float strength) {
	int x = position / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
	m_springs[x].position.y = WATER_HEIGHT - strength;
}
