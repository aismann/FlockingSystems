#include "BoundingRect.h"
#include "Math.h"

BoundingRect::BoundingRect(sf::Vector2f _start, sf::Vector2f _end, float _thickness, sf::Color _color):
	thickness(_thickness)
{
	this->setPoints(_start, _end);

	for (int i = 0; i < 5; ++i) {
		this->vertices[i].color = _color;
	}
}

BoundingRect::~BoundingRect()
{
}

void BoundingRect::setPoints(sf::Vector2f _start, sf::Vector2f _end)
{
	using namespace fe::math;
	sf::Vector2f perp = normalize(perp2D(_end - _start));

	this->vertices[0].position = this->vertices[4].position = _start - perp * thickness;
	this->vertices[1].position = _start + perp * thickness;
	this->vertices[2].position = _end + perp * thickness;
	this->vertices[3].position = _end - perp * thickness;
}

void BoundingRect::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
	_target.draw(&vertices[0], 5, sf::LineStrip, _state);
}
