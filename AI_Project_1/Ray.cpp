#include "Ray.h"
#include "Math.h"
#include "Console.h"

Ray::Ray(sf::Vector2f _start, sf::Vector2f _end)
{
	const float THICKNESS = 2.f;

	using namespace fe::math;
	sf::Vector2f perp = normalize(perp2D(_end - _start));

	this->vertices[0] = sf::Vertex(_start - perp * THICKNESS);
	this->vertices[1] = sf::Vertex(_start + perp * THICKNESS);
	this->vertices[2] = sf::Vertex(_end + perp * THICKNESS);
	this->vertices[3] = sf::Vertex(_end - perp * THICKNESS);
}

Ray::~Ray()
{
}

void Ray::onUpdate(double _dt)
{
	this->currTimer -= _dt;
	this->updateColor();

	if (currTimer <= 0) {
		this->setDeleted();
	}
}

void Ray::onDraw(sf::RenderTarget& _target)
{
	_target.draw(&vertices[0], 4, sf::Quads);
}

void Ray::updateColor()
{
	float fade = fe::math::max(currTimer / startTimer, 0.f);

	for (int i = 0; i < 4; ++i) {
		this->vertices[i].color = sf::Color(int(fade * 255.0), 0, 0);
	}
}
