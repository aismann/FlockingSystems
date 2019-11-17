#include "Node2D.h"
#include "Scene.h"

Node2D::Node2D(std::shared_ptr<Scene> _scene, sf::Vector2f _origin):
	origin(_origin),
	rotation(0.f)
{
	// Add to scene if any
	if (_scene) {
		_scene->addNode(shared_from_this());
	}
}

Node2D::Node2D(Node2D::Ptr _parent, sf::Vector2f _origin): 
	origin(_origin),
	rotation(0.f) 
{
	// Add parent if any
	if (_parent) {
		setParent(_parent);
	}
}

Node2D::~Node2D()
{
}

void Node2D::onEvent(sf::Event& _event)
{
}

void Node2D::onUpdate(double _dt)
{
}

void Node2D::onDraw(sf::RenderTarget& _target)
{
}

void Node2D::setOrigin(sf::Vector2f _pos)
{
}

sf::Vector2f Node2D::getOrigin()
{
	return sf::Vector2f();
}

void Node2D::setRotation(float _rot)
{
}

float Node2D::getRotation()
{
	return 0.0f;
}

void Node2D::setParent(Node2D::Ptr _parent)
{
	// Delete previous parent
	if (this->parent) {
		this->parent->removeChild(shared_from_this());
	}

	// Attach new parent
	this->parent = _parent;
	this->parent->addChild(shared_from_this());
}

void Node2D::addChild(Node2D::Ptr _child)
{
	auto it = std::find(this->children.begin(), this->children.end(), _child);
	if (it != this->children.end()) {
		return;
	}

	this->children.push_back(_child);
}

void Node2D::removeChild(Node2D::Ptr _child)
{
	children.erase(std::remove(children.begin(), children.end(), _child), children.end());
}
