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

	m_metaBallsRenderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_metaBallsSprite.setTexture(m_metaBallsRenderTexture.getTexture());
	//TODO: if screen scrolling then likely need to also setView for this renderTexture

	m_alphaTestShader = &GameData::getInstance().getAsset<sf::Shader>("alphaTest");
	m_alphaTestShader->setUniform("threshold", 0.7f);
	m_alphaTestShader->setUniform("texture", sf::Shader::CurrentTexture);
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
	//temp
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mouse = sf::Mouse::getPosition();
		splash(mouse.x, 1.f);
	}

	float viewBot = window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f);
	m_level = window->getView().getCenter().y + WATER_Y_OFFSET;
	m_frontWave.update(dt, viewBot);
	m_wave1.update(dt, viewBot);
	m_wave2.update(dt, viewBot);
	m_wave3.update(dt, viewBot);


	//update particles
	for (int i = 0; i < m_particles.size(); i++) {
		Particle& p = m_particles[i];
		p.velocity += sf::Vector2f(0.f, GRAVITY * dt);
		p.position += p.velocity * dt;
		p.sprite.setPosition(p.position);
		//TODO: this line here and in splash() needs to be better thought out?
		int x = p.position.x / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
		//TODO: have some variable that calculates bottom of the screen instead of dooing it multiple times
		if (p.velocity.y > 0.f && p.position.y > (window->getView().getCenter().y + (SCREEN_HEIGHT * 0.5f) - m_frontWave.getSpringY(x)))
			p.alive = false;
	}
	if (!m_particles.empty()) {
		m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), [](const Particle& p) {
			return !p.alive;
		}), m_particles.end());
	}

	//update metaballs now that particles have updated
	m_metaBallsRenderTexture.clear(sf::Color(WATER_TOP_COLOR.r, WATER_TOP_COLOR.g, WATER_TOP_COLOR.b, 0.f)); //todo: this should go in draw menthod?
	for (int i = 0; i < m_particles.size(); i++) {
		m_metaBallsRenderTexture.draw(m_particles[i].sprite);// , sf::BlendAdd);
	}
	m_metaBallsRenderTexture.display();
}

void Water::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_frontWave);
	target.draw(m_metaBallsSprite, m_alphaTestShader);
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
	if (strength > 1.f)
		strength = 1.f;
	else if (strength < 0.f)
		strength = 0.f;
	sf::Vector2f particleStart = m_frontWave.splash(position, strength);
	for (int i = 0; i < SPLASH_PARTICLES; i++) {
		Particle p;
		p.sprite.setTexture(GameData::getInstance().getAsset<sf::Texture>("particle"));
		p.sprite.setOrigin(p.sprite.getGlobalBounds().width * 0.5f, p.sprite.getGlobalBounds().height * 0.5f);
		p.position = particleStart;
		p.sprite.setColor(WATER_TOP_COLOR);

		//todo: move velocity calculation to the same place as position calculation...
		float speed = SPLASH_PARTICLES_SPEED * strength;
		p.velocity = sf::Vector2f(Helpers::randomNumberF(-0.7f, 0.7f), -1.f);
		Helpers::normalisedVmult(p.velocity, (SPLASH_PARTICLES_BASE_SPEED + (SPLASH_PARTICLES_SPEED * strength)) * Helpers::randomNumberF(0.3f, 1.f));
		
		m_particles.push_back(p);
	}
}
