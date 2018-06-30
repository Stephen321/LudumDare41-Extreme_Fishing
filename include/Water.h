#pragma once

#include "SFML/Graphics.hpp"
#include "Spriter.h"
#include "Wave.h"
#include "MimicWave.h"

class Water : public sf::Drawable {
public:
	Water(const sf::RenderWindow* _window);
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void drawBackground(sf::RenderTarget& target) const;
	int getLevel() const;
	void splash(float position, float strength);
private:
	struct Particle {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Sprite sprite;
		bool alive = true;
	};

	sf::Clock m_timer;
	const sf::RenderWindow* window;
	float m_level;
	sf::Shader* m_alphaTestShader;
	std::vector<Particle> m_particles;
	sf::RenderTexture m_metaBallsRenderTexture;
	sf::Sprite m_metaBallsSprite;

	Wave m_frontWave;
	MimicWave m_wave1;
	MimicWave m_wave2;
	MimicWave m_wave3;
};