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
	void checkCollisions(const std::vector<Platform>& platforms);
	sf::IntRect getBoundingBox() const;
	bool getAttemptingToFish() const;
	void setFailedAttempt();
	void setSuccessfulAttempt(int length, float time);
	const bool* getQte() const;
	void handleEvents(const sf::Event& ev);
	sf::Vector2f getRodEnd() const;
private:
	void updateCoreLogic(float dt);
	void updateQTE(float dt);
	se::EntityInstance* m_entity;
	sf::Vector2f m_velocity;
	sf::Clock m_spaceTimer;
	sf::Clock m_launchFisherTimer;
	sf::Clock m_qteTimer;
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
	std::deque<sf::Keyboard::Key> m_qteKeys;
};
