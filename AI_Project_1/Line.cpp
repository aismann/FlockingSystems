#include "Line.h"
#include "Math.h"

Line::Line(sf::Vector2f _start, sf::Vector2f _end, float _thickness, sf::Color _color):
	thickness(_thickness)
{
	this->setPoints(_start, _end);

	for (int i = 0; i < 4; ++i) {
		this->vertices[i].color = _color;
	}
}

Line::Line(sf::Vector2f _start, sf::Vector2f _end):
	Line(_start, _end, 1.f, sf::Color(255,0,0))
{
}

Line::~Line()
{
}

void Line::setPoints(sf::Vector2f _start, sf::Vector2f _end)
{
	using namespace fe::math;
	sf::Vector2f perp = normalize(perp2D(_end - _start));

	this->vertices[0].position = _start - perp * thickness;
	this->vertices[1].position = _start + perp * thickness;
	this->vertices[2].position = _end + perp * thickness;
	this->vertices[3].position = _end - perp * thickness;
}

void Line::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
	_target.draw(&vertices[0], 4, sf::Quads, _state);
}
