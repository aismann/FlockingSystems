#include "Node2D.h"
#include "Scene.h"

namespace fe {

	Node2D::Node2D(sf::Vector2f _position) :
		Node()
	{
		this->setPosition(_position);
	}

	Node2D::~Node2D()
	{
	}

	void Node2D::onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state)
	{
		_state.transform *= getTransform();

		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseDraw(_target, _state);
		}

		onDraw(_target, _state);
	}

	sf::Transform Node2D::getGlobalTransform()
	{
		if (auto ptr = this->parent.lock()) {
			return ptr->getGlobalTransform() * this->getTransform();
		}
		else {
			return this->getTransform();
		}
	}
}