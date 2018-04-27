#include "Water.h"
#include "GameData.h"
#include "Helpers.h"

Water::Water(const sf::RenderWindow* _window)
	: window(_window) {
	//m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("water")->getNewEntityInstance("Water");
	//m_entity->setCurrentAnimation("Idle");
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;

	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		Spring s;
		s.position.x = (SCREEN_WIDTH / WATER_SPRINGS_COUNT) * i;
		s.position.y = WATER_HEIGHT;
		m_springs[i] = s;
	}
	m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
}

void Water::start() {
	m_springs[0].position.y = 100;
}

void Water::update(float dt) {
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	//m_entity->setPosition(se::point(0.f, m_level));
	//m_entity->setTimeElapsed(dt);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		m_springs[0].position.y = 200;
	}

	const int WATER_PROP_LOOPS = 10;
	const float SPREAD = 0.4;
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
		if (i < WATER_SPRINGS_COUNT - 1)
			m_springs[i + 1].position.y += deltas[i].y;
	}

	for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
		float x = WATER_HEIGHT - m_springs[i].position.y;
		sf::Vector2f acceleration = sf::Vector2f(0.f, WATER_SPRING_K * x);
		m_springs[i].velocity += acceleration;
		m_springs[i].velocity -= m_springs[i].velocity * WATER_SPRING_DAMPENER;
		if (Helpers::getLength(m_springs[i].velocity) < 0.01f)
			m_springs[i].velocity.x = m_springs[i].velocity.y = 0.f;
		m_springs[i].position += m_springs[i].velocity * dt;
	}
}

void Water::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	//m_entity->render();

	sf::CircleShape s(10);
	s.setOrigin(5, 5);
	s.setFillColor(sf::Color(0, 0, 255, 128));
	m_vertices.clear();
	sf::Vertex v1, v2;
	sf::Vector2f pos = sf::Vector2f(m_springs[0].position.x, m_level - m_springs[0].position.y);
	v1.position = pos;
	v1.color = sf::Color(0, 0, 128);

	v2.position = pos + sf::Vector2f(0.f, WATER_HEIGHT);
	v2.color = sf::Color(0, 0, 255);
	m_vertices.append(v1);
	m_vertices.append(v2);

	for (int i = 1; i < WATER_SPRINGS_COUNT - 1; i++) {
		sf::Vector2f pos = sf::Vector2f(m_springs[i].position.x, m_level - m_springs[i].position.y);
		s.setPosition(pos);
		sf::Vertex v1, v2;
		v1.position = pos;
		v1.color = sf::Color(0, 0, 128);

		v2.position = pos + sf::Vector2f(0.f,  WATER_HEIGHT);
		v2.color = sf::Color(0, 0, 255);

		m_vertices.append(v1);
		m_vertices.append(v2);
		target.draw(s);
	}
	target.draw(m_vertices);
}

int Water::getLevel() const {
	return m_level;
}
