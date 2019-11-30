#pragma once
#include <memory>
#include <SFML/System.hpp>

#include "ColliderCircle.h"

namespace fe {
	class Node2D;

	/*********** Contact properties */
	struct Contact {
		// Game objects
		std::shared_ptr<Node2D> myGameObject;
		std::shared_ptr<Node2D> otherGameObject;

		// Collision properties
		sf::Vector2f			normal;
		sf::Vector2f			conctactPoint;
		float					collisionDepth;
	};

	/*********** Resolve for different colliders */
	std::shared_ptr<Contact> checkColliderCollision(std::shared_ptr<ColliderCircle> _collider1, std::shared_ptr<ColliderCircle> _collider2);
	
}

