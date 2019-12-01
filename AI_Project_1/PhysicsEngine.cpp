#include "PhysicsEngine.h"
#include "PhysicNode.h"
#include "CollisionResolver.h"
#include "Math.h"

#include "Console.h"

namespace fe {

	PhysicsEngine::PhysicsEngine()
	{
	}

	PhysicsEngine::~PhysicsEngine()
	{
	}

	void PhysicsEngine::addBody(std::shared_ptr<PhysicNode> _node)
	{
		auto it = std::find(bodies.begin(), bodies.end(), _node);
		if (it == bodies.end()) {
			bodies.push_back(_node);
		}
	}

	void PhysicsEngine::removeBody(std::shared_ptr<PhysicNode> _node)
	{
		auto it = std::find(bodies.begin(), bodies.end(), _node);
		if (it != bodies.end()) {
			bodies.erase(it);
		}
	}

	std::vector<std::shared_ptr<PhysicNode>> PhysicsEngine::raycast(sf::Vector2f _point, sf::Vector2f _dir)
	{
		auto bodiesList = std::vector<std::shared_ptr<PhysicNode>>();

		for (auto body : this->bodies) {
			if (body->isDeleted() || body->isDisabled()) {
				continue;
			}

			auto collider = std::static_pointer_cast<ColliderCircle>(body->getCollider());

			if (!collider) {
				C_WARNING("Invalid collider type or collider not set, skipped!");
				continue;
			}

			auto colliderPos = collider->getGlobalTransform().transformPoint(sf::Vector2f());
			auto toCollider =  colliderPos - _point;
			auto lenSq = math::lengthSquare(math::perp(toCollider, _dir));
			auto vectDir = math::dotProduct(toCollider, _dir);

			if (lenSq < math::pow(collider->getRadius(), 2) && vectDir > 0) {
				bodiesList.push_back(body);
			}
		}

		return bodiesList;
	}

	void PhysicsEngine::onUpdate(double _dt)
	{
		deleteTimer -= _dt;
		if (deleteTimer <= 0) {
			auto pastEndIt = std::remove_if(bodies.begin(), bodies.end(), [](auto body) {return body->isDeleted(); });
			bodies.erase(pastEndIt, bodies.end());

			deleteTimer = deleteInterval;
		}

		for (int i = 0; i < this->bodies.size(); ++i) {
			for (int j = i+1; j < this->bodies.size(); ++j) {
				auto bodyPtr1 = bodies[i];
				auto bodyPtr2 = bodies[j];

				if (bodyPtr1->isDeleted() || bodyPtr2->isDeleted() || bodyPtr1->isDisabled() || bodyPtr1->isDisabled()) {
					continue;
				}
				
				checkCollision(bodyPtr1, bodyPtr2);
			}
		}
	}

	void PhysicsEngine::checkCollision(std::shared_ptr<PhysicNode> _node1, std::shared_ptr<PhysicNode> _node2)
	{
		auto collider1 = std::static_pointer_cast<ColliderCircle>(_node1->getCollider());
		auto collider2 = std::static_pointer_cast<ColliderCircle>(_node2->getCollider());

		std::shared_ptr<Contact> result = checkColliderCollision(collider1, collider2);

		if (result) {
			_node1->onBeginContact(result);
		}
	}

}
