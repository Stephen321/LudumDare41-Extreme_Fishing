#include "Water.h"
#include "GameData.h"
#include "Helpers.h"
//#define _USE_MATH_DEFINES 
//#include <cmath>
//todo: why doesnt the above work
#define PI 3.141592653589793

Water::Water(const sf::RenderWindow* _window)
	: window(_window) { 
	//m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("water")->getNewEntityInstance("Water");
	//m_entity->setCurrentAnimation("Idle");
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
	m_blurShader = &GameData::getInstance().getAsset<sf::Shader>("blur");
	m_blurShader->setUniform("blur_radius", 0.1f);
	m_blurShader->setUniform("texture", sf::Shader::CurrentTexture);
}

void Water::start() {
	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		Spring s;
		s.position.x = ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT) * i;
		s.position.y = WATER_HEIGHT;
		m_springs[i] = s;
	}
	m_particles.clear();
	createSines();
	m_timer.restart();
}

void Water::createSines() { 
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

float Water::overlapSines(float x) const {
	float result = 0;
	for (int i = 0; i < NUM_BACKGROUND_WAVES; i++) {
		result += m_sineOffsets[i]
				+ m_sineAmplitudes[i]
				* sin(x * m_sineStretches[i] + m_offset * m_offsetStretches[i]);
	}
	return result;
}

void Water::update(float dt) {
	m_offset++;

	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	//m_entity->setPosition(se::point(0.f, m_level));
	//m_entity->setTimeElapsed(dt);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mouse = sf::Mouse::getPosition();
		splash(mouse.x, mouse.y / 10);
	}

	//update particles
	for (int i = 0; i < m_particles.size(); i++) {
		Particle& p = m_particles[i];
		p.velocity += sf::Vector2f(0.f, GRAVITY * dt);
		p.position += p.velocity * dt;
		p.sprite.setPosition(p.position);
		//TODO: this line here and in splash() needs to be better thought out?
		int x = p.position.x / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
		//TODO: have some variable that calculates bottom of the screen instead of dooing it multiple times
		if (p.position.y > m_level + WATER_HEIGHT || p.position.y > (window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f) - m_springs[x].position.y))
			p.alive = false;
	}
	if (!m_particles.empty()) {
		m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), [](const Particle& p) {
			return !p.alive;
		}), m_particles.end());
	}
	///////////////////////

	const int WATER_PROP_LOOPS = 10;
	const float SPREAD = 0.45;
	sf::Vector2f deltas[WATER_SPRINGS_COUNT];
	for (int i = 0; i < WATER_PROP_LOOPS; i++) {
		for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
			if (i > 0) {
				deltas[i].x = SPREAD * (m_springs[i].position.y - m_springs[i - 1].position.y);
				m_springs[i - 1].velocity.y += deltas[i].x; //
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
	//todo: make these constants
	sf::CircleShape s(5);
	s.setOrigin(2.5, 2.5);
	s.setFillColor(sf::Color(0, 0, 255, 128));
	m_vertices.clear();
	sf::Vertex v1, v2;
	sf::Vector2f pos = sf::Vector2f(m_springs[0].position.x, (m_level + WATER_HEIGHT) - m_springs[0].position.y + (overlapSines(m_springs[0].position.x)));
	v1.position = pos;
	v1.color = WATER_TOP_COLOR;

	v2.position = pos + sf::Vector2f(0.f, m_springs[0].position.y - (overlapSines(m_springs[0].position.x)));
	v2.color = WATER_BOT_COLOR;
	m_vertices.append(v1);
	m_vertices.append(v2);

	for (int i = 1; i < WATER_SPRINGS_COUNT - 1; i++) {
		float wave = (overlapSines(m_springs[i].position.x)); //TODO: wave gets added to the draw and not part of the sppring physics
		sf::Vector2f pos = sf::Vector2f(m_springs[i].position.x, (m_level + WATER_HEIGHT) - m_springs[i].position.y + wave);
		s.setPosition(pos);
		sf::Vertex v1, v2;
		v1.position = pos;
		v1.color = WATER_TOP_COLOR;

		v2.position = pos + sf::Vector2f(0.f, m_springs[i].position.y - wave);
		v2.color = WATER_BOT_COLOR;

		m_vertices.append(v1);
		m_vertices.append(v2);
		//target.draw(s);
	}
	target.draw(m_vertices);

	for (int i = 0; i < m_particles.size(); i++) {
		target.draw(m_particles[i].sprite);// , m_blurShader);
		//target.draw(m_particles[i].sprite, m_blurShader);
	}
}

int Water::getLevel() const {
	return m_level;
}

void Water::splash(float position, float strength) {
	int x = position / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);

	
	float wave = (overlapSines(m_springs[x].position.x)); //get what the wave is at this point
	sf::Vector2f start;
	start.x = m_springs[x].position.x;
	start.y = window->getView().getCenter().y + (0.5f * SCREEN_HEIGHT) - m_springs[x].position.y - wave;

	for (int i = 0; i < SPLASH_PARTICLES; i++) {
		//make new particles
		Particle p;
		p.sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>("particle"));
		p.position = start;
		float speed = SPLASH_PARTICLES_SPEED * strength;
		p.velocity = sf::Vector2f(Helpers::randomNumberF(-0.8f, 0.8f) * speed, -speed);
		Helpers::limit(p.velocity, SPLASH_PARTICLES_SPEED * strength * Helpers::randomNumberF(0.2f, 1.f));

		p.sprite.setColor(sf::Color(0, 0, 255, 128));
		m_particles.push_back(p);
	}
	m_springs[x].position.y = WATER_HEIGHT - strength;
}
