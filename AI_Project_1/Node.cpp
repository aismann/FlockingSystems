#include "Node.h"

namespace fe {
	/* Static variables*/
	int Node::NODE_ID = 0;

	Node::Node() :
		deleted(false),
		disabled(false),
		nodeId(Node::NODE_ID++)
	{
	}

	Node::~Node()
	{
	}

	void Node::onBaseEvent(sf::Event& _event)
	{
		onEvent(_event);

		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseEvent(_event);
		}
	}

	void Node::onBaseUpdate(double _dt)
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

	void Node::onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state)
	{
		// Non spatial Node cannot be drawn
	}

	void Node::onInit()
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node::onExit()
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node::onEvent(sf::Event& _event)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node::onUpdate(double _dt)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Node::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	sf::Transform Node::getGlobalTransform()
	{
		if (auto ptr = this->parent.lock()) {
			return ptr->getGlobalTransform() * sf::Transform();
		}
		else {
			return sf::Transform();
		}
	}

	void Node::addChild(std::shared_ptr<Node> _child)
	{
		auto it = std::find(this->children.begin(), this->children.end(), _child);
		if (it != this->children.end()) {
			return;
		}

		this->reqChildren.push_back(_child);
	}

	void Node::setDeleted()
	{
		this->deleted = true;
	}

	bool Node::isDeleted()
	{
		return this->deleted;
	}

	void Node::setDisabled(bool _val)
	{
		this->disabled = _val;
	}

	bool Node::isDisabled()
	{
		return this->disabled;
	}

	int Node::getNodeId()
	{
		return this->nodeId;
	}

}