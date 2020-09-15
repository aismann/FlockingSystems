#include "CollisionResolver.h"
#include "Math.h"
#include "ColliderCircle.h"

namespace fe {
	void flipContact(Contact& _contact)
	{
		_contact.normal = (-_contact.normal);
		std::swap(_contact.myGameObject, _contact.otherGameObject);
	};

	bool checkColliderCollision(Contact& _result, std::shared_ptr<ColliderCircle> _collider1, std::shared_ptr<ColliderCircle> _collider2)
	{
		sf::Vector2f posCollider1 = _collider1->getGlobalTransform().transformPoint(sf::Vector2f());
		sf::Vector2f posCollider2 = _collider2->getGlobalTransform().transformPoint(sf::Vector2f());

		float dist = math::length(posCollider1 - posCollider2);
		float radiusSum = _collider1->getRadius() + _collider2->getRadius();
		
		if (dist <= radiusSum) {
			_result.normal = math::normalize(posCollider1 - posCollider2);
			_result.conctactPoint = (posCollider1 + posCollider2) / 2.f;
			_result.collisionDepth = (radiusSum - dist) / 2.f;

			return true;
		}

		return false;
	}
}