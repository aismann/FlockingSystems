#include "Node2D.h"
#include "Scene.h"

namespace fe {

	Node2D::Node2D(sf::Vector2f _position) :
		deleted(false),
		disabled(false)
	{
		this->setPosition(_position);
	}

	Node2D::~Node2D()
	{
	}

	void Node2D::onBaseEvent(sf::Event& _event)
	{
		onEvent(_event);

		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseEvent(_event);
		}
	}

	void Node2D::onBaseUpdate(double _dt)
	{
		onUpdate(_dt);

		// Update all childs
		for (int i = 0; i < children.size(); /* conditional */) {
			auto child = children[i];

			if (child->isDisabled()) {
				continue;
			}
			
			child->onBaseUpdate(_dt);

			if (child->isDeleted()) {
				int lastIt = children.size() - 1;
				std::swap(children[i], children[lastIt]);

				child->onExit();
				children.pop_back();
			}
			else {
				i++;
			}
		}

		// Add requested childs
		for (auto ch : reqChildren) {
			ch->onInit();
			ch->parent = shared_from_this();
			children.push_back(ch);
		}
		reqChildren.clear();
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

		onDraw(_target);
	}

	void Node2D::onInit()
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node2D::onExit()
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node2D::onEvent(sf::Event& _event)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node2D::onUpdate(double _dt)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node2D::onDraw(sf::RenderTarget& _target)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	sf::Transform Node2D::getGlobalTransform()
	{
		if (auto ptr = this->parent.lock()) {
			return ptr->getTransform() * this->getTransform();
		}
		else {
			return this->getTransform();
		}
	}

	void Node2D::addChild(std::shared_ptr<Node2D> _child)
	{
		auto it = std::find(this->children.begin(), this->children.end(), _child);
		if (it != this->children.end()) {
			return;
		}

		this->reqChildren.push_back(_child);
	}

	void Node2D::setDeleted()
	{
		this->deleted = true;
	}

	bool Node2D::isDeleted()
	{
		return this->deleted;
	}

	void Node2D::setDisabled(bool _val)
	{
		this->disabled = _val;
	}

	bool Node2D::isDisabled()
	{
		return this->disabled;
	}

}