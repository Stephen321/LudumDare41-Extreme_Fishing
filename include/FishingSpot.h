#pragma once

#include "GameObject.h"
#include "Spriter.h"
#include "Constants.h"

class FishingSpot : public GameObject, public sf::Drawable {
public:
	FishingSpot();
	void start(const sf::Vector2f start, int x, int qteMax, bool isShark);
	void update(float dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	int getX() const;
	sf::IntRect getBoundingBox() const;
	int getLength() const;
	float getTime() const;
	void setBeingFished(bool beingFished);
	bool getIsShark() const;
	bool justDied();
	void setDying();
	void setX(float x);
private:
	se::EntityInstance* m_entity;
	se::EntityInstance* m_sharkSpot;
	se::EntityInstance* m_fishSpot;
	sf::Clock m_activeTimer;
	bool m_dying;
	int m_timeToBeActive;
	int m_x;
	int m_length;
	bool m_justDied;
	float m_time;
	bool m_beingFished;
	bool m_isShark;
	std::string m_fadeInAnim;
	std::string m_fadeBubblesAnim;
	std::string m_fadeOutAnim;
};