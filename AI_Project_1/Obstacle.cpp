#include "Obstacle.h"
#include "Shape2D.h"
#include "ColliderCircle.h""

Obstacle::Obstacle(sf::Vector2f _position):
	PhysicNode(_position)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::onInit()
{
	// Add shape
	auto shape = std::make_shared<sf::CircleShape>(30.f, 30);
	shape->setFillColor(sf::Color(200, 200, 200));
	shape->setOrigin(sf::Vector2f(30.f, 30.f));

	auto planet = std::make_shared<fe::Shape2D>();
	planet->setShape(shape);
	this->addChild(planet);

	// Add collider
	this->setCollider(std::make_shared<fe::ColliderCircle>(30.f));
}
