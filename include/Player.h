#pragma once

#include <deque>
#include "GameObject.h"
#include "Spriter.h"
#include "Platform.h"

class Player : public GameObject, public sf::Drawable {
public:
	Player();
	void start(const sf::Vector2f& start);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void drawQte() const;
	void checkCollisions(const std::vector<Platform>& platforms);
	sf::IntRect getBoundingBox() const;
	bool getAttemptingToFish() const;
	void setFailedAttempt();
	void setSuccessfulAttempt(int length, float time);
	const bool* getQte() const;
	void handleEvents(const sf::Event& ev);
	sf::Vector2f getRodEnd() const;
	void setQteFishSpot(const sf::Vector2f& qteFishSpot);
	const sf::Vector2f* getPositionPtr() const;
	float getQteTime() const;
	void hit();
	bool getHit() const;
private:
	void updateCoreLogic(float dt);
	void updateQTE(float dt);
	se::EntityInstance* m_entity;
	se::EntityInstance* m_qteEnt;
	se::EntityInstance* m_speechEnt;
	sf::Vector2f m_qteFishSpot;
	sf::Clock m_displaySpeechTimer;
	sf::Vector2f m_velocity;
	sf::Clock m_jumpTimer;
	sf::Clock m_launchFisherTimer;
	sf::Clock m_qteTimer;
	sf::Clock m_hitTimer;
	mutable bool m_hit; //m
	bool m_spaceHeld;
	float m_speed;
	bool m_grounded;
	float m_lastY;
	bool m_crouching;
	bool m_blending;
	bool m_fishing;
	bool m_attemptingToFish;
	bool m_launchFish;
	bool m_qte;
	float m_qteTime;
	float m_jumpForce;
	int m_qteLength;
	std::deque<sf::Keyboard::Key> m_qteKeys;
};
