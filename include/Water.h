#pragma once

#include "SFML\Graphics.hpp"
#include "Spriter.h"
#include "Constants.h"

class Water : public sf::Drawable {
public:
	Water(const sf::RenderWindow* _window);
	void start();
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	int getLevel() const;
	void splash(float position, float strength);
private:
	void createSines();
	float overlapSines(float x) const;
	struct Spring {
		sf::Vector2f position; 
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
	};

	//////////////////////////////////////////////////////////////////
	//https://gamedev.stackexchange.com/questions/44547/how-do-i-create-2d-water-with-dynamic-waves
	//////////////////////////////////////////////////////////////////
	// A phase difference to apply to each sine
	float m_offset;
	// Amounts by which a particular sine is offset
	std::vector<float> m_sineOffsets;
	// Amounts by which a particular sine is amplified
	std::vector<float> m_sineAmplitudes;
	// Amounts by which a particular sine is stretched
	std::vector<float> m_sineStretches;
	// Amounts by which a particular sine's offset is multiplied
	std::vector<float> m_offsetStretches;
	// Set each sine's values to a reasonable random value
	//////////////////////////////////////////////////////////////////

	sf::Clock m_timer;
	se::EntityInstance* m_entity;
	const sf::RenderWindow* window;
	mutable sf::VertexArray m_vertices;
	float m_level;
	Spring m_springs[WATER_SPRINGS_COUNT];

	//todo: first particle attempts uses sf::Sprites, switch to quads + vertex array?
	struct Particle {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Sprite sprite;
		bool alive = true;
	};
	sf::Shader* m_blurShader;
	std::vector<Particle> m_particles;
};