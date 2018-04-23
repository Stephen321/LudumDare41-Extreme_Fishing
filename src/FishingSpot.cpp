#include "FishingSpot.h"
#include "GameData.h"
#include "Constants.h"
#include "Helpers.h"

using namespace Helpers;

FishingSpot::FishingSpot(){
	m_alive = false;
	m_entity = GameData::getInstance().getAsset<se::SpriterModel*>("fishingSpot")->getNewEntityInstance("FishingSpot");
}

void FishingSpot::start(const sf::Vector2f start, int x, int qteMax) {
	m_position = start;
	m_x = x;
	m_entity->setCurrentAnimation(FISHINGSPOT_FADEIN_ANIM);
	m_entity->setCurrentTime(0.f);
	m_activeTimer.restart();
	m_alive = true;
	m_beingFished = false;
	m_length = randomNumber(QTE_MIN, qteMax);
	int l = qteMax - QTE_MIN;
	for (int i = 1; i < l; i++) {
		if (rand() % (i + 1) == 0) {
			m_length = QTE_MIN + i - 1;
			break;
		}
	}
	//double r = (double)rand() / RAND_MAX; //0 - 1.f
	//float baseWeight = 1.f / l;
	//std::vector<float> weights;
	//for (int i = 0; i < l; i++) {
	//	weights.push_back((baseWeight + (((l - (i + 1)) * baseWeight) * (baseWeight * 1.5f))) / (baseWeight * 3.f));
	//}
	//for (int i = 0; i < l ; i++) {
	//	if (r < weights[i]) {
	//		m_length = QTE_MIN + i;
	//		break;
	//	}
	//}//length is more likely to be lower number

	m_time = (int)(m_length * QTE_TIME_PER_KEY) + randomNumber(-QTE_RAND_OFFSET, QTE_RAND_OFFSET);
	//m_time = ((m_length / ((QTE_MIN + QTE_MAX) * 0.5f)) * QTE_MIN_TIME) + randomNumberF(-QTE_RAND_OFFSET * 0.35f, QTE_RAND_OFFSET * 2.5f);
	//m_timeToBeActive = (m_time * 1.5f) + randomNumberF(-FISHINGSPOT_ACTIVE_TIME * 0.75f, FISHINGSPOT_ACTIVE_TIME * 1.25f);
	//m_timeToBeActive = fmax(m_timeToBeActive, FISHINGSPOT_ACTIVE_TIME);
	m_timeToBeActive = m_length + randomNumber(-FISHINGSPOT_ACTIVE_OFFSET, FISHINGSPOT_ACTIVE_OFFSET); 
	m_timeToBeActive *= 2;
	m_timeToBeActive = m_timeToBeActive + (m_timeToBeActive % FISHINGSPOT_ACTIVE_TIME_MULTIPLE);
	if (m_timeToBeActive > FISHINGSPOT_MAXACTIVE)
		m_timeToBeActive = FISHINGSPOT_MAXACTIVE;

	cout << "m_length: " << m_length;
	cout << ", m_time: " << m_time;
	cout << ", m_timeToBeActive: " << m_timeToBeActive << endl;
}

void FishingSpot::update(float dt) {
	if (m_entity->currentAnimationName() == FISHINGSPOT_FADEIN_ANIM) {
		if (m_entity->animationJustLooped()) {
			se::changeAnimation(m_entity, FISHINGSPOT_BUBBLE_ANIM);
		}
	}
	m_position.y += SCROLL_SPEED * dt;
	m_entity->setPosition(se::vectorToPoint(m_position));
	m_entity->setTimeElapsed(dt);	
	if (m_activeTimer.getElapsedTime().asSeconds() > m_timeToBeActive && m_beingFished == false)
		m_alive = false;
}

void FishingSpot::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (!m_alive)
		return;
	m_entity->render();
}

int FishingSpot::getX() const {
	return m_x;
}

sf::IntRect FishingSpot::getBoundingBox() const {
	sf::IntRect bb;
	bb.left = (int)m_position.x;
	bb.top = (int)m_position.y;
	bb.width = FISHINGSPOT_SIZE_X * TILE_SIZE;
	bb.height = (int)(FISHINGSPOT_SIZE_Y * TILE_SIZE);
	return bb;
}

int FishingSpot::getLength() const {
	return m_length;
}

float FishingSpot::getTime() const {
	return m_time;
}

void FishingSpot::setBeingFished(bool beingFished) {
	m_beingFished = beingFished;
}
