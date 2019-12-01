#include "PhysicNode.h"

#include "Engine.h"
#include "PhysicsEngine.h"
#include "Collider.h"

namespace fe {

	PhysicNode::PhysicNode(sf::Vector2f _position):
		Node2D(_position)
	{
	}

	PhysicNode::~PhysicNode()
	{
	}

	void PhysicNode::setCollider(std::shared_ptr<Collider> _collider)
	{
		// Set collider
		this->collider = _collider;
		this->collider->setGameObject(shared_from_this());

		// Add do physic queuee
		fe::EngineInstance.getPhysicEngine()->addBody(std::static_pointer_cast<PhysicNode>(shared_from_this()));
	}

	std::shared_ptr<Collider> fe::PhysicNode::getCollider()
	{
		return this->collider;
	}

}
