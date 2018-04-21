#include "GameObject.h"

GameObject::GameObject()
	: m_alive(true) {

}

bool GameObject::getAlive() const {
	return m_alive;
}

void GameObject::setAlive(bool alive) {
	m_alive = alive;
}

sf::Vector2f GameObject::getPosition() {
	return m_position;
}
