#include "Obstacle.h"
#include "Shape2D.h"
#include "ColliderCircle.h""
#include "Line.h"

Obstacle::Obstacle(sf::Vector2f _position):
	PhysicNode(_position),
	radius(30.f)
{
}

Obstacle::~Obstacle()
{
}

float Obstacle::getRadius()
{
	return radius;
}

void Obstacle::onInit()
{
	// Add shape
	auto shape = std::make_shared<sf::CircleShape>(this->radius, 30);
	shape->setFillColor(sf::Color(200, 200, 200));
	shape->setOrigin(sf::Vector2f(this->radius, this->radius));

	auto shapeNode = std::make_shared<fe::Shape2D>();
	shapeNode->setShape(shape);
	this->addChild(shapeNode);

	// Add collider
	this->setCollider(std::make_shared<fe::ColliderCircle>(this->radius));
}
