#include "PhysicNode.h"

namespace fe {

	PhysicNode::PhysicNode(sf::Vector2f _origin)
	{

	}

	PhysicNode::~PhysicNode()
	{
	}

	void PhysicNode::setCollider(std::shared_ptr<Collider> _collider)
	{
		this->collider = _collider;
	}

	std::shared_ptr<Collider> fe::PhysicNode::getCollider()
	{
		return this->collider;
	}

}
