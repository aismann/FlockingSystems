#include "Enemy.h"

#include "Shape2D.h"
#include "Console.h"
#include "Engine.h"
#include "Ray.h"
#include "Line.h"
#include "BoundingRect.h"
#include "PhysicsEngine.h"
#include "ColliderCircle.h"
#include "EnemyController.h"

Enemy::Enemy(std::shared_ptr<MainScene> _scene, sf::Vector2f _position):
	PhysicNode(_position),
	mainScene(_scene),
	radius(7.f),
	tagged(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::onInit()
{
	this->setMaxSpeed(150.f);
	this->setMaxForce(400.f);

	/*** Get visible world size ***/
	auto window = fe::EngineInstance.getMainWindow();
	worldSizeEnd = window->getSize();

	/*** Unit circle ***/
	auto shape = std::make_shared<sf::CircleShape>(radius, 10);
	shape->setFillColor(sf::Color(50, 50, 0));
	shape->setOrigin(sf::Vector2f(radius, radius));

	auto circle = std::make_shared<fe::Shape2D>();
	circle->setShape(shape);
	this->addChild(circle);

	/*** Set collider **/
	this->setCollider(std::make_shared<fe::ColliderCircle>(radius));

	/*** Set AI ***/
	this->controller = std::make_shared<EnemyController>(this);
	this->addChild(this->controller);

	//this->controller->addBehaviourMode(EnemyController::HIDE);
	//this->controller->addBehaviourMode(EnemyController::PURSUIT);
	//this->controller->addBehaviourMode(EnemyController::WANDER);
	//this->controller->addBehaviourMode(EnemyController::OBSTACLE_AVOIDANCE);

	this->controller->addBehaviourMode(EnemyController::COHESION);
	this->controller->addBehaviourMode(EnemyController::ALLIGNMENT);
	this->controller->addBehaviourMode(EnemyController::SEPARATION);

	/*** DEBUG VECTORS **/
	debugVectorVelocity = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 200, 0));
	debugVectorHeading = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 100, 200));
	debugVectorSide = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(200, 100, 0));
	debugVectorAvoid = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 255, 255));
	debugVectorWander = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(255, 255, 0));
	debugVectorHide = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(255, 255, 255));
	//this->addChild(debugVectorVelocity);
	this->addChild(debugVectorHeading);
	//this->addChild(debugVectorSide);
	//this->addChild(debugVectorAvoid);
	//this->addChild(debugVectorWander);
	//this->addChild(debugVectorHide);

	debugVectorCohesion = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(200, 0, 0));
	debugVectorSeparation = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 200, 0));
	debugVectorAlignment = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 0, 200));
	this->addChild(debugVectorCohesion);
	this->addChild(debugVectorSeparation);
	this->addChild(debugVectorAlignment);

	debugBoundingRect = std::make_shared<BoundingRect>(sf::Vector2f(), sf::Vector2f(), radius, sf::Color(255, 0, 0));
	//this->addChild(debugBoundingRect);
}

void Enemy::onUpdate(double _dt)
{	
	auto acc = this->controller->getSteeringForce() / this->getMass();
	this->velocity += acc * (float)_dt;
	
	if (fe::math::lengthSquare(this->velocity) > 0.00000001){
		this->velocity = fe::math::truncate(this->velocity, this->maxSpeed);
		this->setHeading(this->velocity);
	}

	// DEBUG
	debugVectorVelocity->setPoints(sf::Vector2f(), this->velocity);
	debugVectorHeading->setPoints(sf::Vector2f(), this->heading * 20.f);
	debugVectorSide->setPoints(sf::Vector2f(), this->side * 20.f);
	
	this->debugBoundingRect->setPoints(sf::Vector2f(), this->heading * 80.f);

	this->move(this->velocity * (float)_dt);
	this->wrapScreen();
}

void Enemy::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
}

void Enemy::rayHit()
{
	this->setDeleted();
}

void Enemy::setTag(bool _tag)
{
	this->tagged = _tag;
}

bool Enemy::isTagged()
{
	return this->tagged;
}

void Enemy::wrapScreen()
{
	auto pos = this->getPosition();

	if (pos.x < 0.f) { pos.x += worldSizeEnd.x; }
	if (pos.x > worldSizeEnd.x) { pos.x -= worldSizeEnd.x; }
	if (pos.y < 0.f) { pos.y += worldSizeEnd.y; }
	if (pos.y > worldSizeEnd.y) { pos.y -= worldSizeEnd.y; }

	this->setPosition(pos);
}
