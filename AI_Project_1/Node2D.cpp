#include "Node2D.h"
#include "Scene.h"

Node2D::Node2D(Scene* _scene, sf::Vector2f _origin):
	parent(nullptr)
{
	setOrigin(_origin);

	// Add to scene
	if (_scene) {
		_scene->addNode(this);
	}
}

Node2D::Node2D(Node2D* _parent, sf::Vector2f _origin):
	parent(nullptr)
{
	setOrigin(_origin);

	// Add parent
	if (_parent) {
		setParent(_parent);
	}
}

Node2D::~Node2D()
{
	for (auto child : children) {
		delete child;
	}
}

void Node2D::onBaseEvent(sf::Event& _event)
{
	onEvent(_event);

	for (auto child : children) {
		child->onBaseEvent(_event);
	}
}

void Node2D::onBaseUpdate(double _dt)
{
	onUpdate(_dt);

	for (auto child : children) {
		child->onBaseUpdate(_dt);
	}
}

void Node2D::onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
	_state.transform *= getTransform();
	for (auto child : children) {
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

void Node2D::setParent(Node2D* _parent)
{
	// Delete previous parent
	if (this->parent) {
		this->parent->removeChild(this); // TODO: Move ptr 
	}

	// Attach new parent
	this->parent = _parent;
	_parent->addChild(this);
}

void Node2D::addChild(Node2D* _child)
{
	auto it = std::find(this->children.begin(), this->children.end(), _child);
	if (it != this->children.end()) {
		return;
	}

	this->children.push_back(_child);
}

void Node2D::removeChild(Node2D* _child)
{
	children.erase(std::remove(children.begin(), children.end(), _child), children.end());
}
