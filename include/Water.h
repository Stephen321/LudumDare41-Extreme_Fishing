#pragma once

#include "SFML/Graphics.hpp"
#include "Spriter.h"
#include "Constants.h"
#include "Helpers.h"

//#define _USE_MATH_DEFINES 
//#include <cmath>
//todo: why doesnt the above work
#define PI 3.141592653589793

class Wave : public sf::Drawable {
public:
	Wave() {
		m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
	}
	void start(float viewBot) {
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
	void update(float dt, float viewBot) {
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
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
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
			target.draw(s);
		}
	}

	float getWave(int idx) const {
		//returns the wave at the current idx
		if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
			return 0.f;
		return (overlapSines(m_springs[idx].position.x));
	}

	float getSpringY(int idx) const {
		if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
			return 0.f;
		return m_springs[idx].position.y;
	}

	float getCurrentHeight(int idx) const {
		if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
			return 0.f;
		float wave = getWave(idx); //wave y
		float spring = getSpringY(idx);//spring physics y
		return spring + wave; //current height of the wave
	}

	sf::Vector2f splash(float xPos, float strength) {
		//todo: move velocity calculation to the same place as position calculation...
		//todo refactor this
		int x = xPos / ((float)SCREEN_WIDTH / WATER_SPRINGS_COUNT);
		float wave = (overlapSines(m_springs[x].position.x)); //get what the wave is at this point
		sf::Vector2f start;
		start.x = m_springs[x].position.x;
		start.y = m_viewBot - m_springs[x].position.y - wave;

		m_springs[x].position.y = WATER_HEIGHT - (SPLASH_PARTICLES_WAVE_MIN_STRENGTH + (strength * SPLASH_PARTICLES_WAVE_STRENGTH));
		
		//SPLASH_PARTICLES_X_MAXOFFSET
		return start;
	}

	sf::VertexArray getVertices() const {
		return m_vertices;
	}

private:

	//https://gamedev.stackexchange.com/questions/44547/how-do-i-create-2d-water-with-dynamic-waves
	void createSines() {
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
	float overlapSines(float x) const {
		float result = 0;
		for (int i = 0; i < NUM_BACKGROUND_WAVES; i++) {
			result += m_sineOffsets[i]
				+ m_sineAmplitudes[i]
				* sin(x * m_sineStretches[i] + m_offset * m_offsetStretches[i]);
		}
		return result;
	}
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

class MimicWave: public sf::Drawable {
public:
	MimicWave(const Wave* _wave, const MimicWave* _behindWave, float hMult, float yOffset)
	 : wave(_wave)
	 , behindWave(_behindWave)
	 , m_hMult(hMult)
	 , m_yOffset(yOffset) {
		m_vertices = sf::VertexArray(sf::PrimitiveType::TrianglesStrip, WATER_SPRINGS_COUNT * 2);
	}
	void start(const sf::Color& topColor, const sf::Color& botColor) {
		//IMPORTANT: TODO: these colours are interpolated all the way to the bottom of the screen...
		//change the bottom y position of this to always match the top y of the layer in front of it...
		m_vertices = wave->getVertices();
		for (int i = 0; i < WATER_SPRINGS_COUNT; i++) {
			m_vertices[i * 2].color = botColor;
			m_vertices[(i * 2) + 1].color  = topColor;
		}
	}
	void update(float dt, float viewBot) {
		m_waterLevel = viewBot - WATER_HEIGHT; //TODO: this and the one in Wave have to be the same 
		m_viewBot = viewBot;

		//------------------------------------------------------------------------------
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
		//------------------------------------------------------------------------------
	}

	//---------------------------------------------------------------

	float getCurrentHeight(int idx) const {
		if (idx < 0 || idx >= WATER_SPRINGS_COUNT)
			return 0.f;
		return m_viewBot - m_vertices[idx * 2].position.y;
	}
	//---------------------------------------------------------------

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
		target.draw(m_vertices);
	}
private:
	const Wave* wave;
	const MimicWave* behindWave;
	sf::VertexArray m_vertices;
	float m_waterLevel;
	float m_viewBot;
	float m_hMult;
	float m_yOffset;
};

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