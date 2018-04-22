#include "Player.h"
#include "GameData.h"
#include "Constants.h"

Player::Player()
	: m_speed(PLAYER_MAXSPEED)
	, m_grounded(false)
	, m_spaceHeld(false)
	, m_blending(false)
	, m_crouching(false)
	, m_fishing(false)
	, m_launchFish(false)
	, m_attemptingToFish(false) 
	, m_qte(false) {
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("fisher")->getNewEntityInstance("Player");
}

void Player::start(const sf::Vector2f& start) {
	m_position = start;
	m_grounded = true;
	m_crouching = false;
	m_crouching = false;
	m_fishing = false;
	m_attemptingToFish = false;
	m_qte = false;
	m_launchFish = false;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
}

void Player::update(float dt) {
	sf::IntRect bb= getBoundingBox();
	m_lastY = bb.top + bb.height - (m_velocity.y * dt);


	if (m_qte == false && m_grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		//launch fishing!
		if (m_attemptingToFish == false) {
			m_launchFish = true;
			m_launchFisherTimer.restart();
			//play fish launch animation here
			se::changeAnimation(m_entity, PLAYER_FISHLAUNCH_ANIM);
			m_entity->setCurrentTime(0.f);
		}
	}

	if (m_entity->currentAnimationName() == PLAYER_FISHLAUNCH_ANIM) {
		if (m_entity->animationJustFinished(true)) {
			se::changeAnimation(m_entity, PLAYER_IDLE_FISHING_ANIM);
			m_attemptingToFish = true;
			m_launchFish = false;
		}
	}

	if (m_qte == false && m_attemptingToFish == false) {
		if (m_launchFish == false || m_launchFisherTimer.getElapsedTime().asSeconds() > PLAYER_LAUNCH_FISH_MINTIME)
			updateCoreLogic(dt);
	}
	else if (m_qte)
		updateQTE(dt);

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
			if (m_lastY < bb.top) {
				m_grounded = true;
				if (m_entity->currentAnimationName() == PLAYER_FALL_ANIM) {
					se::changeAnimation(m_entity, PLAYER_LAND_ANIM);
				}
				break;
			}
			else if (m_position.y > bb.top + bb.height) {
				falling = true; //special case
			}
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

bool Player::getAttemptingToFish() const {
	return m_attemptingToFish;
}

void Player::setFailedAttempt() {
	if (m_attemptingToFish) {
		m_attemptingToFish = false;
		//fail animation here
	}
}

void Player::setSuccessfulAttempt() {
	if (m_attemptingToFish) {
		m_attemptingToFish = false;
		//success animation here
		
		//start qte
		m_qteTimer.restart();
		m_qte = true;
		//todo: make time and keys harder and determined by type of fishing spot
		m_qteTime = PLAYER_QTE_TIME; 
		//set up qte keys 
		m_qteKeys.clear();
		m_qteKeys.push_back(sf::Keyboard::Key::W);
		m_qteKeys.push_back(sf::Keyboard::Key::A);
		m_qteKeys.push_back(sf::Keyboard::Key::S);
		m_qteKeys.push_back(sf::Keyboard::Key::D);
	}
}

const bool * Player::getQte() const {
	return &m_qte;
}

void Player::handleEvents(const sf::Event & ev) {
	if (m_qte) {
		if (ev.type == sf::Event::KeyReleased && ev.key.code == m_qteKeys.front()){
			m_qteKeys.pop_front();
			cout << "popping: " << ev.key.code << endl;
			if (m_qteKeys.size() == 0) {
				//qte successful!
				cout << "qte success" << endl;
				m_qte = false;
				return;
			}
		}
		else if (ev.type == sf::Event::KeyPressed && ev.key.code != m_qteKeys.front()) {
			//qte failed! wrong key
			m_qte = false;
			cout << "qte fail, wrong key: " << ev.key.code << endl;
			return;
		}
	}
}

sf::Vector2f Player::getRodEnd() const {
	return sf::Vector2f(m_position.x + (-m_entity->getScale().x * PLAYER_ROD_OFFSET_X), getBoundingBox().top - PLAYER_ROD_OFFSET_Y);
}


void Player::updateCoreLogic(float dt) {
	//user input
	sf::Vector2f dir;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		dir.x = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		dir.x = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (m_crouching == false) {
			m_crouching = true;
			if (m_entity->currentAnimationName() == PLAYER_RUN_ANIM) {
				se::changeAnimation(m_entity, PLAYER_IDLE_CROUCH_ANIM);
			}
			else if (m_entity->currentAnimationName() == PLAYER_IDLE_ANIM) {
				se::changeAnimation(m_entity, PLAYER_IDLE_CROUCH_ANIM, 200.f);
			}
		}
	}
	else if (m_crouching) {
		m_crouching = false;
		m_blending = false;
	}
	if (m_grounded && m_crouching) {
		dir.x = 0; //cant move left or right while crouching
	}

	if (dir.x != 0.f) {
		m_entity->setScale(se::point(-dir.x, 1.f));
		if (m_grounded) {
			se::changeAnimation(m_entity, PLAYER_RUN_ANIM);
		}
	}
	else if (m_grounded && m_crouching == false) {
		if (m_blending == false && m_entity->currentAnimationName() == PLAYER_IDLE_CROUCH_ANIM) {
			se::changeAnimation(m_entity, PLAYER_IDLE_ANIM, 200.f);
			m_blending = true;
		}
		else if (m_entity->currentAnimationName() == PLAYER_RUN_ANIM ||
				 m_entity->currentAnimationName() == PLAYER_JUMP_ANIM) {
			se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
		}
	}

	bool crouchFell = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (m_grounded) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				//move down through platform
				m_grounded = false;
				crouchFell = true;
				m_position.y += 1.f;
				se::changeAnimation(m_entity, PLAYER_CROUCH_FALL_ANIM);
				m_entity->setCurrentTime(0.f);
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
			//jump if still grounded
			if (m_grounded) {
				se::changeAnimation(m_entity, PLAYER_JUMP_ANIM);
				m_entity->setCurrentTime(0.f);
				m_velocity.y = PLAYER_JUMPFORCE + ((PLAYER_JUMPFORCE * 0.5f) * m);
				m_grounded = false;
			}
		}
	}

	if (m_entity->currentAnimationName() == PLAYER_JUMP_ANIM) {
		if (m_entity->animationJustLooped()) {
			se::changeAnimation(m_entity, PLAYER_FALL_ANIM);
		}
	}
	else if (m_entity->currentAnimationName() == PLAYER_LAND_ANIM) {
		if (m_entity->animationJustLooped()) {
			if (m_crouching)
				se::changeAnimation(m_entity, PLAYER_IDLE_CROUCH_ANIM);
			else
				se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
		}
	}
	else if (m_entity->currentAnimationName() == PLAYER_CROUCH_FALL_ANIM) {
		if (m_entity->animationJustLooped()) {
			if (m_grounded == false)
				se::changeAnimation(m_entity, PLAYER_FALL_ANIM);
			else
				se::changeAnimation(m_entity, PLAYER_LAND_ANIM);
		}
	}
	else if (m_entity->currentAnimationName() == PLAYER_IDLE_FISHING_ANIM) {
		se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
	}
	//else if (m_entity->currentAnimationName() == PLAYER_FISH_ANIM) {
	//	if (m_entity->animationJustLooped()) {
	//		se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
	//	}
	//}

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

	if (!crouchFell && !m_grounded && m_velocity.y > 0.f &&
			(m_entity->currentAnimationName() == PLAYER_RUN_ANIM ||
			m_entity->currentAnimationName() == PLAYER_IDLE_CROUCH_ANIM)) {
		se::changeAnimation(m_entity, PLAYER_FALL_ANIM);
	}

	//update position
	m_position += m_velocity * dt;
}

void Player::updateQTE(float dt) {
	cout << "QTE: ";
	for (int i = 0; i < m_qteKeys.size(); i++) {
		cout << m_qteKeys[i] << " ";
	}
	cout << endl;
	
	if (m_qteTimer.getElapsedTime().asSeconds() > m_qteTime) {
		//qte fauked! ran out of time
		cout << "qte fail, time ran out" << endl;
		m_qte = false;
	}
}
