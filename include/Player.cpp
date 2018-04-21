#include "Player.h"
#include "GameData.h"
#include "Constants.h"

Player::Player()
	: m_speed(PLAYER_MAXSPEED)
	, m_grounded(false)
	, m_spaceHeld(false) {
	m_entity = GameData::getInstance().getAsset<se::EntityInstance*>("Player");
}

void Player::start(const sf::Vector2f& start) {
	m_position = start;
	m_grounded = false;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	m_entity->setCurrentAnimation("Idle");
}

void Player::update(float dt) {
	sf::IntRect bb= getBoundingBox();
	m_lastY = bb.top + bb.height - (m_velocity.y * dt);
	//user input
	sf::Vector2f dir;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		dir.x = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		dir.x = 1;
	}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
	//	dir.y = -1;
	//}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
	//	dir.y = 1;
	//}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (m_grounded) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				//move down through platform
				m_grounded = false;
				m_position.y += 1.f;
			}
			else {
				if (m_spaceHeld == false) {
					m_spaceHeld = true;
					m_spaceTimer.restart();
				}
			}
		}
	}
	else {
		if (m_spaceHeld) { //space released!
			m_spaceHeld = false;
			float heldFor = m_spaceTimer.getElapsedTime().asSeconds();
			float m = fmin(heldFor, PLAYER_MAXSPACEMULT) / PLAYER_SPACEMULT;
			//jump
			m_velocity.y = PLAYER_JUMPFORCE + ((PLAYER_JUMPFORCE * 0.5f) * m);
			m_grounded = false;
		}
	}

	//velocity change
	sf::Vector2f gravity;
	if (m_grounded) {
		m_velocity.y = 0.f;
	}
	else {
		gravity = sf::Vector2f(0.f, GRAVITY);
	}
	m_velocity += gravity * dt;
	if (dir.x != 0.f)
		m_velocity.x = dir.x * m_speed;
	else //no x drag or accel 
		m_velocity.x = 0.f;

	//drag
	//if (dir.x == 0.f) {
	//	if (abs(m_velocity.x) > PLAYER_DRAGTHRESHOLD)
	//		m_velocity.x *= (m_velocity.x < 0.f) ? -PLAYER_DRAG * dt : PLAYER_DRAG * dt;
	//	else
	//		m_velocity.x = 0.f;
	//}

	//limit max speed
	//if (m_velocity.y != 0.f) {
	//	if (m_velocity.y < PLAYER_SPEEDTHRESHOLD)
	//		limit(m_velocity, PLAYER_MAXSPEED);
	//	else
	//		limit(m_velocity, PLAYER_MAXSPEED * 4.f); //if falling then max speed is greater
	//}

	//temp collision detected
	//if (m_position.y > 800.f) {
	//	m_position.y = 800.f;
	//	m_velocity.y = 0.f;
	//	m_grounded = true;
	//}

	//update position
	m_position += m_velocity * dt;
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	m_entity->render();
}

void Player::checkCollisions(const std::vector<Platform>& platforms) {
	bool falling = true;
	for (int i = 0; i < platforms.size(); i++) {
		sf::IntRect bb = platforms[i].getBoundingBox();
		sf::IntRect playerBB = getBoundingBox();
		if (bb.intersects(playerBB)) {
			falling = false;
			if (m_lastY < bb.top)
				m_grounded = true;
		}
	}
	if (falling)
		m_grounded = false;
}

sf::IntRect Player::getBoundingBox() const {
	sf::IntRect bb;
	bb.left = (int)m_position.x - (TILE_SIZE * 0.5f);
	bb.top = (int)m_position.y - (TILE_SIZE * 2.f);
	bb.width = PLAYER_SIZE_X * TILE_SIZE;
	bb.height = (int)(PLAYER_SIZE_Y * TILE_SIZE);
	return bb;
}
