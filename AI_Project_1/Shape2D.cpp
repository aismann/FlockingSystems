#include "Shape2D.h"
#include <SFML/Graphics/Shape.hpp>

namespace fe {

	Shape2D::Shape2D(sf::Vector2f _origin) :
		Node2D(_origin),
		shape(nullptr)
	{

	}

	Shape2D::~Shape2D()
	{
	}

	void Shape2D::onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state)
	{
		Node2D::onBaseDraw(_target, _state);

		if (this->shape) {
			_target.draw(*(this->shape), _state);
		}
	}

	void Shape2D::setShape(std::shared_ptr<sf::Shape> _shape)
	{
		this->shape = _shape;
	}

	std::shared_ptr<sf::Shape> Shape2D::getShape()
	{
		return this->shape;
	}

}