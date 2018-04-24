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
	m_qteEnt = GameData::getInstance().getAsset<se::SpriterModel*>("QTE")->getNewEntityInstance("QTE");
	m_speechEnt = GameData::getInstance().getAsset<se::SpriterModel*>("QTE")->getNewEntityInstance("QTE");
}

void Player::start(const sf::Vector2f& start) {
	m_position = start;
	m_grounded = true;
	m_crouching = false;
	m_fishing = false;
	m_attemptingToFish = false;
	m_qte = false;
	m_launchFish = false;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	se::changeAnimation(m_entity, PLAYER_IDLE_ANIM);
	m_jumpForce = PLAYER_JUMPFORCE_EXTRA;
	m_hit = false;
}

void Player::update(float dt) {
	sf::IntRect bb= getBoundingBox();
	m_lastY = (bb.top + bb.height - (m_velocity.y * dt)); // -(SCROLL_SPEED * dt * 2);


	if (m_qte == false && m_grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		//launch fishing!
		if (m_attemptingToFish == false) {
			m_launchFish = true;
			m_launchFisherTimer.restart();
			se::changeAnimation(m_speechEnt, QTE_ATTEMPT_ANIM);
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



	if (m_qte) {
		m_qteEnt->setPosition(se::vectorToPoint(
			sf::Vector2f(
				m_qteFishSpot.x + 5.f,
				m_qteFishSpot.y - ((1.1f + FISHINGSPOT_SIZE_Y) * TILE_SIZE)
			)
		));
	}
	m_qteEnt->setTimeElapsed(dt);

	if (m_launchFish || m_attemptingToFish || m_qte || 
		((m_speechEnt->currentAnimationName() == QTE_SUCCESS_ANIM ||
		  m_speechEnt->currentAnimationName() == QTE_FAIL_ANIM) &&
		 m_displaySpeechTimer.getElapsedTime().asSeconds() < QTE_STATUS_DISPLAY_TIME))
		m_speechEnt->setPosition(se::vectorToPoint(
			sf::Vector2f(
				m_position.x + 5.f,
				m_position.y - ((1.1f + PLAYER_SIZE_Y) * TILE_SIZE)
			)
		));
	m_speechEnt->setTimeElapsed(dt);
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_hit) {
		if (m_hitTimer.getElapsedTime().asSeconds() < 2.5f) {
			if (m_entity->getAlpha() > 0.5f)
				m_entity->setAlpha(0.5f);
		}
		else {
			m_entity->setAlpha(1.f);
			m_hit = false;
		}
	}
	m_entity->render();
	if (m_qte)
		m_qteEnt->render();
	if (m_entity->currentAnimationName() == PLAYER_IDLE_FISHING_ANIM ||
		m_entity->currentAnimationName() == PLAYER_FISHLAUNCH_ANIM || 
		((m_speechEnt->currentAnimationName() == QTE_SUCCESS_ANIM ||
		  m_speechEnt->currentAnimationName() == QTE_FAIL_ANIM) && 
		 m_displaySpeechTimer.getElapsedTime().asSeconds() < QTE_STATUS_DISPLAY_TIME))
		m_speechEnt->render();
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
		se::changeAnimation(m_speechEnt, QTE_FAIL_ANIM);
	}
}

void Player::setSuccessfulAttempt(int length, float time) {
	if (m_attemptingToFish) {
		m_attemptingToFish = false;
		//success animation here
		
		//start qte
		m_qteTimer.restart();
		m_qte = true;
		m_qteTime = time;
		//set up qte keys 
		m_qteKeys.clear();
		int k = -1;
		for (int i = 0; i < length; i++) {
			int newK = rand() % POSSIBLE_KEYS_SIZE;
			while (newK == k) {
				newK = rand() % POSSIBLE_KEYS_SIZE;
			}
			m_qteKeys.push_back(POSSIBLE_QTE_KEYS[k]);
			k = newK;
		}
		m_qteLength = length;
		se::changeAnimation(m_qteEnt, QTE_ANIM_PREFIX + keyToStr	(m_qteKeys.front()));
	}
}

const bool * Player::getQte() const {
	return &m_qte;
}

void Player::handleEvents(const sf::Event & ev) {
	if (m_qte) {
		if (ev.type == sf::Event::KeyReleased && ev.key.code == m_qteKeys.front()){
			m_qteKeys.pop_front();
			cout << "popping: " << keyToStr(ev.key.code) << endl;
			if (m_qteKeys.size() == 0) {
				//qte successful!
				se::changeAnimation(m_speechEnt, QTE_SUCCESS_ANIM);
				m_displaySpeechTimer.restart();
				m_qte = false;
				int score = (m_qteLength * BASE_SCORE) - (m_qteLength * DECR_SCORE * (m_qteTimer.getElapsedTime().asSeconds() / m_qteTime));
				GameData::getInstance().score += score;
				return;
			}
			else {
				se::changeAnimation(m_qteEnt, QTE_ANIM_PREFIX + keyToStr(m_qteKeys.front()));
			}
		}
		else if (ev.type == sf::Event::KeyPressed && ev.key.code != m_qteKeys.front()) {
			//qte failed! wrong key
			m_qte = false;
			m_displaySpeechTimer.restart();
			se::changeAnimation(m_speechEnt, QTE_FAIL_ANIM);
			return;
		}
	}
}

sf::Vector2f Player::getRodEnd() const {
	return sf::Vector2f(m_position.x + (-m_entity->getScale().x * PLAYER_ROD_OFFSET_X), getBoundingBox().top - PLAYER_ROD_OFFSET_Y);
}

void Player::setQteFishSpot(const sf::Vector2f & qteFishSpot) {
	m_qteFishSpot = qteFishSpot;
}

const sf::Vector2f * Player::getPositionPtr() const {
	return &m_position;
}

float Player::getQteTime() const {
	return m_qteTime - m_qteTimer.getElapsedTime().asSeconds();
}

void Player::hit() {
	m_hit = true;
	m_hitTimer.restart();
}

bool Player::getHit() const {
	return m_hit;
}


void Player::updateCoreLogic(float dt) {
	m_launchFish = false;

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
	bool justJumped = false;
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
				//first time jumping
				m_jumpForce = PLAYER_JUMPFORCE_EXTRA;
				m_velocity.y = PLAYER_JUMPFORCE;
				se::changeAnimation(m_entity, PLAYER_JUMP_ANIM);
				m_entity->setCurrentTime(0.f);
				m_grounded = false;
				justJumped = true;
				m_jumpTimer.restart();
			}
		}
		else if (m_jumpTimer.getElapsedTime().asSeconds() < PLAYER_MAX_SPACEHOLD) {
			//we can increase our jump force here
			m_jumpForce += (PLAYER_JUMPFORCE_INC * dt);
			m_velocity.y += m_jumpForce * dt;
		}
	}

	if (m_entity->currentAnimationName() == PLAYER_JUMP_ANIM) {
		if (m_entity->animationJustLooped()) {
			se::changeAnimation(m_entity, PLAYER_FALL_ANIM);
		}
	}
	else if (m_entity->currentAnimationName() == PLAYER_LAND_ANIM) {
		if (m_entity->animationJustLooped() || m_crouching) {
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
	else if (justJumped == false && m_entity->currentAnimationName() == PLAYER_IDLE_FISHING_ANIM) {
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
		cout << keyToStr(m_qteKeys[i]) << " ";
	}
	cout << endl;
	
	if (m_qteTimer.getElapsedTime().asSeconds() > m_qteTime) {
		//qte failed! ran out of time
		m_displaySpeechTimer.restart();
		se::changeAnimation(m_speechEnt, QTE_FAIL_ANIM);
		m_qte = false;
	}
}
