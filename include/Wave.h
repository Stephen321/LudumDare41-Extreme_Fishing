#pragma once

#include "SFML/Graphics.hpp"
#include "Constants.h"
//#define _USE_MATH_DEFINES 
//#include <cmath>
//todo: why doesnt the above work
#define PI 3.141592653589793

class Wave : public sf::Drawable {
public:
	Wave();
	void start(float viewBot);
	void update(float dt, float viewBot);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	float getWave(int idx) const;
	float getSpringY(int idx) const;
	float getCurrentHeight(int idx) const;
	sf::Vector2f splash(float xPos, float strength);
	sf::VertexArray getVertices() const;

private:
	void createSines();
	float overlapSines(float x) const;
	struct Spring {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
	};
	float m_offset;
	std::vector<float> m_sineOffsets;
	std::vector<float> m_sineAmplitudes;
	std::vector<float> m_sineStretches;
	std::vector<float> m_offsetStretches;

	sf::VertexArray m_vertices;
	Spring m_springs[WATER_SPRINGS_COUNT];
	float m_waterLevel;
	float m_viewBot;
};
