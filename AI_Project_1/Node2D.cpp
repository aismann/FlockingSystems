#include "Node2D.h"
#include "Scene.h"

namespace fe {

	Node2D::Node2D(sf::Vector2f _origin) :
		deleted(false),
		disabled(false)
	{
		setOrigin(_origin);
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

		// Delete flagged childs
		auto pastEndIt = std::remove_if(children.begin(), children.end(), [](auto node) {return node->isDeleted(); });
		children.erase(pastEndIt, children.end());

		// Update all childs
		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseUpdate(_dt);
		}

		// Add requested childs
		for (auto ch : reqChildren) {
			//s->init();
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

	void Node2D::setParent(std::shared_ptr<Node2D> _parent)
	{
		// Delete previous parent
		if (auto ptr = this->parent.lock()) {
			ptr->removeChild(shared_from_this());
		}

		// Attach new parent
		this->parent = _parent;
		if (_parent) {
			_parent->addChild(shared_from_this());
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

	void Node2D::removeChild(std::shared_ptr<Node2D> _child)
	{
		children.erase(std::remove(children.begin(), children.end(), _child), children.end());
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