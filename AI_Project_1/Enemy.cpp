#include "Enemy.h"
#include "Shape2D.h"
#include "Console.h"

#include "Engine.h"
#include "PhysicsEngine.h"
#include "ColliderCircle.h"

Enemy::Enemy(sf::Vector2f _position):
	PhysicNode(_position)
{
}

Enemy::~Enemy()
{
}

void Enemy::onInit()
{
	const float SIZE = 7.f;

	// Unit arrow
	auto shape = std::make_shared<sf::CircleShape>(SIZE, 10);
	shape->setFillColor(sf::Color(50, 200, 0));
	shape->setOrigin(sf::Vector2f(SIZE / 2.f, SIZE / 2.f));

	auto circle = std::make_shared<fe::Shape2D>();
	circle->setShape(shape);
	this->addChild(circle);

	// Set collider
	this->setCollider(std::make_shared<fe::ColliderCircle>(SIZE));
}

void Enemy::rayHit()
{
	C_INFO("Enemy deleted!");
	this->setDeleted();
}
