#include "Shape2D.h"
#include <SFML/Graphics/Shape.hpp>

Shape2D::Shape2D(Scene* _scene, sf::Vector2f _origin):
	Node2D(_scene, _origin),
	shape(nullptr)
{

}

Shape2D::Shape2D(Node2D* _parent, sf::Vector2f _origin):
	Node2D(_parent, _origin),
	shape(nullptr)
{

}

Shape2D::~Shape2D()
{
	delete this->shape;
}

void Shape2D::onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
	Node2D::onBaseDraw(_target, _state);

	if (this->shape) {
		_target.draw(*(this->shape), _state);
	}
}

void Shape2D::setShape(sf::Shape* _shape)
{
	this->shape = _shape;
}

sf::Shape* Shape2D::getShape()
{
	return this->shape;
}
