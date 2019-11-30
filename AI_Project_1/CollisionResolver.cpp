#include "CollisionResolver.h"
#include "Math.h"
#include "ColliderCircle.h"

namespace fe {
	std::shared_ptr<Contact> checkColliderCollision(std::shared_ptr<ColliderCircle> _collider1, std::shared_ptr<ColliderCircle> _collider2)
	{
		auto result = std::shared_ptr<Contact>();

		float dist = math::length(_collider1->getPosition() - _collider2->getPosition());
		float radiusSum = (_collider1->getRadius() + _collider2->getRadius(), 2);
		if (dist <= radiusSum) {
			result->normal = math::normalize(_collider2->getPosition() - _collider1->getPosition());
			result->conctactPoint = (_collider1->getPosition() + _collider2->getPosition()) / 2.f;
			result->collisionDepth = (radiusSum - dist) / 2.f;
		}

		return result;
	}
}