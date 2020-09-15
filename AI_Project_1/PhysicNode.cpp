#include "PhysicNode.h"

#include "Engine.h"
#include "PhysicsEngine.h"
#include "Collider.h"

#include <cassert>

namespace fe {

	PhysicNode::PhysicNode(sf::Vector2f _position) :
		Node2D(_position)
	{
		this->velocity = sf::Vector2f();

		this->heading = sf::Vector2f(1.f, 0.f);
		this->side = math::perp2D(this->heading);

		this->maxSpeed = 100.f;
		this->maxForce = 100.f;
		this->mass = 1.0;
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

	void PhysicNode::setDeleted()
	{
		Node2D::setDeleted();

		// Remove from physic queuee
		if (this->collider) {
			fe::EngineInstance.getPhysicEngine()->removeBody(std::static_pointer_cast<PhysicNode>(shared_from_this()));
		}
	}

	void PhysicNode::setLayerFlags(int _flags)
	{
		layerFlag = _flags;
	}

	bool PhysicNode::isFlag(int _flags)
	{
		return (bool)(layerFlag & _flags);
	}

	sf::Vector2f PhysicNode::pointToWorldSpace(sf::Vector2f _local)
	{
		sf::Vector2f result;
		sf::Vector2f pos = this->getPosition();

		result.x = heading.x * _local.x + side.x * _local.y	+ pos.x;
		result.y = heading.y * _local.x	+ side.y * _local.y	+ pos.y;

		return result;
	}

	sf::Vector2f PhysicNode::pointToLocalSpace(sf::Vector2f _world)
	{
		sf::Vector2f result;
		sf::Vector2f pos = this->getPosition();

		float tX = heading.x * pos.x + heading.y * pos.y;
		float tY = side.x * pos.x + side.y * pos.y;

		result.x = heading.x * _world.x + heading.y * _world.y - tX;
		result.y = side.x * _world.x + side.y * _world.y - tY;

		return result;
	}

	sf::Vector2f PhysicNode::vectorToWorldSpace(sf::Vector2f _local)
	{
		sf::Vector2f result;

		result.x = heading.x * _local.x + side.x * _local.y;
		result.y = heading.y * _local.x + side.y * _local.y;

		return result;
	}

	sf::Vector2f PhysicNode::vectorToLocalSpace(sf::Vector2f _world)
	{
		sf::Vector2f result;

		result.x = heading.x * _world.x + heading.y * _world.y;
		result.y = side.x * _world.x + side.y * _world.y;

		return result;
	}

	void PhysicNode::setHeading(sf::Vector2f _vect)
	{
		this->heading = math::normalize(_vect);
		this->side = math::perp2D(heading);
	}

}
