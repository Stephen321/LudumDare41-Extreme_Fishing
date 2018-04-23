#pragma once

#include "spriterengine/spriterengine.h"
#include "spriterengine/global/settings.h"
#include "spriterengine/user_override/exampleobjectfactory.h"
#include "spriterengine/user_override/examplefilefactory.h"

namespace se = SpriterEngine;

namespace SpriterEngine {
	SpriterEngine::point inline vectorToPoint(const sf::Vector2f& vector) {
		return SpriterEngine::point(vector.x, vector.y);
	}
	sf::Vector2f inline pointToVector(SpriterEngine::point p) {
		return sf::Vector2f(p.x, p.y);
	}
	inline void changeAnimation(se::EntityInstance* e, const std::string& name, float blend = 0.f) {
		if (e->currentAnimationName() != name) {
			e->setCurrentAnimation(name, blend);
		}
	}
}