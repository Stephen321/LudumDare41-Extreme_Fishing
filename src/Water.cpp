#include "Water.h"
#include "GameData.h"
#include "Helpers.h"

Water::Water(const sf::RenderWindow* _window)
	: window(_window)
	, m_wave1(&m_frontWave, nullptr, 0.6f, WATER_HEIGHT * 0.2f)
	, m_wave2(&m_frontWave, &m_wave1, 0.4f, WATER_HEIGHT * 0.35f)
	, m_wave3(&m_frontWave, &m_wave2, 0.2f, WATER_HEIGHT * 0.45f) { //todo: remove need for this here
	//m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("water")->getNewEntityInstance("Water");
	//m_entity->setCurrentAnimation("Idle");
	//m_blurShader = &GameData::getInstance().getAsset<sf::Shader>("blur");
	//m_blurShader->setUniform("blur_radius", 0.1f);
	//m_blurShader->setUniform("texture", sf::Shader::CurrentTexture);
}

void Water::start() {
	float viewBot = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f);
	m_frontWave.start(viewBot);
	m_wave1.start(Helpers::colorMult(WATER_SURFACE_COLOR, 1.0f), Helpers::colorMult(WATER_SURFACE_COLOR, 1.0f)); //todo: has to be called after front wave
	m_wave2.start(Helpers::colorMult(WATER_SURFACE_COLOR, 0.99f), Helpers::colorMult(WATER_SURFACE_COLOR, 0.99f));
	m_wave3.start(Helpers::colorMult(WATER_SURFACE_COLOR, 0.98f), Helpers::colorMult(WATER_SURFACE_COLOR, 0.98f));

	m_particles.clear();
	m_timer.restart();
}

void Water::update(float dt) {
	float viewBot = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f);
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	m_frontWave.update(dt, viewBot);
	m_wave1.update(dt, viewBot);
	m_wave2.update(dt, viewBot);
	m_wave3.update(dt, viewBot);

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
		//int x = p.position.x / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
		//TODO: have some variable that calculates bottom of the screen instead of dooing it multiple times
		//if (p.position.y > m_level + WATER_HEIGHT || p.position.y > (window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f) - m_springs[x].position.y))
		//	p.alive = false;
	}
	if (!m_particles.empty()) {
		m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), [](const Particle& p) {
			return !p.alive;
		}), m_particles.end());
	}
}

void Water::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_frontWave);

	for (int i = 0; i < m_particles.size(); i++) {
		target.draw(m_particles[i].sprite);// , m_blurShader);
		//target.draw(m_particles[i].sprite, m_blurShader);
	}
}

void Water::drawBackground(sf::RenderTarget & target) const {
	target.draw(m_wave3);
	target.draw(m_wave2);
	target.draw(m_wave1);
}

int Water::getLevel() const {
	return m_level;
}

void Water::splash(float position, float strength) {
	sf::Vector2f particleStart = m_frontWave.splash(position, strength);
	for (int i = 0; i < SPLASH_PARTICLES; i++) {
		Particle p;
		p.sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>("particle"));
		p.position = particleStart;
		float speed = SPLASH_PARTICLES_SPEED * strength;
		p.velocity = sf::Vector2f(Helpers::randomNumberF(-0.8f, 0.8f) * speed, -speed);
		p.sprite.setColor(sf::Color(0, 0, 255, 128));
		Helpers::limit(p.velocity, SPLASH_PARTICLES_SPEED * strength * Helpers::randomNumberF(0.2f, 1.f));
		m_particles.push_back(p);
	}
}
