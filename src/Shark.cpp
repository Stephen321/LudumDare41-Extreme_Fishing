#include "Shark.h"
#include "GameData.h"
#include "Constants.h"
#include "Helpers.h"
#include "GameData.h"
#include "SceneManager.h"

Shark::Shark(Player* _player)
	: m_speed(SHARK_SPEED) 
	, player(_player) {
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("Shark")->getNewEntityInstance("Shark");
}

void Shark::start(const sf::Vector2f& start, sf::Vector2f target) {
	int flip = 1;
	if (target.x < start.x) {
		flip = -1;
	}
	target.x += flip * ((SHARK_SIZE_X / 2) * TILE_SIZE);
	target.y -= (PLAYER_SIZE_Y + 1) * TILE_SIZE;
	m_position = start;
	m_angle = 0.f;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	m_alive = true;
	se::changeAnimation(m_entity, SHARK_IDLE_ANIM);
	m_entity->setScale(se::point(flip, 1.f));

	float MaxSpeed = 10;
	float Gravity = 9.81;

	sf::Vector2f displacement = (target - start);
	float dist = Helpers::getLength(displacement);
	float T = dist / MaxSpeed;
	m_velocity = 1.0f / T * (displacement)-(0.5f*(sf::Vector2f(0.f, Gravity))*T);

	float maxHeight = start.y - target.y;
	float maxDistance = fabs(start.x - target.x);
	float vSpeed = sqrt(2 * GRAVITY * maxHeight); // calculate the vertical speed
	float totalTime = 2 * vSpeed / GRAVITY; // calculate the total time
	float hSpeed = maxDistance / totalTime; // calculate the horizontal speed
	hSpeed = flip * hSpeed;
	m_velocity = sf::Vector2f(hSpeed, -vSpeed); // launch the projectile!

	//#2
	//float distance = target.x - start.x;
	//float angleToPoint = atan2(target.y - start.y, target.x - start.x);
	//float distanceFactor = 1 / 100;
	//float angleCorrection = ((3.14)*0.18) * (distance * distanceFactor);
	//m_velocity.x = cos(angleToPoint + angleCorrection) * (SHARK_SPEED * 10);
	//m_velocity.y = sin(angleToPoint + angleCorrection) * (SHARK_SPEED * 10);

	//#1
	//float g = GRAVITY / 100.f; // gravity
	//float v = SHARK_SPEED / 100.f; // velocity
	//float x = target.x / 100.f; // target x
	//float y = target.y / 100.f; // target y
	//float s = (v * v * v * v) - g * (g * (x * x) + 2 * y * (v * v)); //substitution
	//m_angle = atan(((v * v) + sqrt(s)) / (g * x)); // launch angle
	
	//m_velocity.x = SHARK_SPEED * cos(m_angle);
	//m_velocity.y = SHARK_SPEED * sin(m_angle);
	float l = Helpers::getLength(m_velocity);
	m_angle = atan2(m_velocity.y / l, m_velocity.x / l);


	//set up collision circles
	float rad = SHARK_COL_RAD;
	float off = se::toRadians(25);
	sf::Vector2f centre = m_position + sf::Vector2f(0.f, (-TILE_SIZE * (SHARK_SIZE_Y * 0.5f)) + rad);
	for (int i = 0; i < SHARK_COL_CIRCLES; i++) {
		m_colRads[i] = rad;
		m_colCentres[i] = m_position;// centre;
		//m_colCentres[i] = Helpers::rotate_point(m_position.x, m_position.y, off, m_colCentres[i]);
		centre.y += rad;
		rad *= 0.9f;
	}
}

void Shark::update(float dt) {
	if (!m_alive)
		return;
	m_velocity += sf::Vector2f(0.f, GRAVITY * dt);
	m_position += m_velocity * dt;
	m_position.y += SCROLL_SPEED * dt;

	float angleBefore = m_angle;
	float l = Helpers::getLength(m_velocity);
	m_angle = atan2(m_velocity.y / l, m_velocity.x / l);
	float angleChange = m_angle - angleBefore;
	for (int i = 0; i < SHARK_COL_CIRCLES; i++) { //better if these were just offsets to position...
		m_colCentres[i] += m_velocity * dt;
		m_colCentres[i].y += SCROLL_SPEED * dt;
		//m_colCentres[i] = Helpers::rotate_point(m_position.x, m_position.y, angleChange, m_colCentres[i]);
		//check if collide with player
		sf::IntRect bb = player->getBoundingBox();
		sf::Vector2f pC = player->getPosition() - sf::Vector2f(0.f, PLAYER_SIZE_Y * 0.5 * TILE_SIZE);
		float dist = Helpers::getLength(pC - m_colCentres[i]);
		if (dist < m_colRads[i] + (bb.width * 1.5f)) {
			//collision happened
			if (player->getHit() == false) {
				player->hit();
				if (!GOD_MODE)
					GameData::getInstance().lives--;
				if (GameData::getInstance().lives == 0) {
					SceneManager::getInstance().changeScene(Scene::Type::GameOverScene);
				}
			}
		}
	}


	m_entity->setAngle(se::toRadians(se::toDegrees(m_angle) + 90.f)); //SpriterEngine::toRadians
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);

	if (m_position.y > player->getPosition().y + 2000)
		m_alive = false;

}

void Shark::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (!m_alive)
		return;
	m_entity->render();
	//sf::CircleShape cs;
	//cs.setFillColor(sf::Color(255, 0, 0, 100));
	//for (int i = 0; i < SHARK_COL_CIRCLES; i++) {
	//	cs.setRadius(m_colRads[i]);
	//	cs.setPosition(m_colCentres[i]);
	//	cs.setOrigin(m_colRads[i], m_colRads[i]);
	//	target.draw(cs);
	//}
}

sf::IntRect Shark::getBoundingBox() const {
	sf::IntRect bb;
	bb.left = (int)m_position.x - (TILE_SIZE * 0.5f);
	bb.top = (int)m_position.y - (TILE_SIZE * 2.f);
	bb.width = SHARK_SIZE_X * TILE_SIZE;
	bb.height = (int)(SHARK_SIZE_Y * TILE_SIZE);
	return bb;
}

float* Shark::getColRads() const {
	return (float*)m_colRads;
}

sf::Vector2f* Shark::getColCentres() const {
	return (sf::Vector2f*)m_colCentres;
}
