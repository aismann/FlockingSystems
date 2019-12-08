#include "Collider.h"
#include "Node2D.h"

namespace fe {
	Collider::Collider()
	{
	}

	Collider::~Collider()
	{
	}

	sf::Transform Collider::getGlobalTransform()
	{
		if (auto ptr = this->gameObject.lock()) {
			return ptr->getGlobalTransform() * this->getTransform();
		}
		else {
			return this->getTransform();
		}
	}

	void Collider::setGameObject(std::shared_ptr<Node> _gameObject)
	{
		this->gameObject = _gameObject;
	}

}