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

// Consts
const sf::Color Enemy::COLOR_BASIC		= sf::Color(0, 200, 0);
const sf::Color Enemy::COLOR_ATTACKING	= sf::Color(200, 0, 0);

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
	/*** Get visible world size ***/
	auto window = fe::EngineInstance.getMainWindow();
	worldSizeEnd = window->getSize();

	/*** Unit circle ***/
	auto shape = std::make_shared<sf::CircleShape>(radius, 10);
	shape->setFillColor(Enemy::COLOR_BASIC);
	shape->setOutlineColor(sf::Color(100, 0, 0));
	shape->setOutlineThickness(1.f);
	shape->setOrigin(sf::Vector2f(radius, radius));

	circle = std::make_shared<fe::Shape2D>();
	circle->setShape(shape);
	this->addChild(circle);

	/*** Set collider **/
	this->setCollider(std::make_shared<fe::ColliderCircle>(radius));
	this->setLayerFlags(PhysicNode::RAY_CAST);

	this->setMaxSpeed(140.f);
	this->setMaxForce(300.f);

	/*** Set AI ***/
	this->controller = std::make_shared<EnemyController>(this);
	this->addChild(this->controller);

	this->controller->addBehaviourMode(EnemyController::PURSUIT);
	this->controller->addBehaviourMode(EnemyController::HIDE);
	this->controller->addBehaviourMode(EnemyController::FORCE_HIDE);
	this->controller->addBehaviourMode(EnemyController::WANDER);
	this->controller->addBehaviourMode(EnemyController::OBSTACLE_AVOIDANCE);
	this->controller->addBehaviourMode(EnemyController::FLEE);

	this->controller->addBehaviourMode(EnemyController::COHESION);
	//this->controller->addBehaviourMode(EnemyController::ALLIGNMENT);
	this->controller->addBehaviourMode(EnemyController::SEPARATION);

	/*** DEBUG VECTORS **/
	//debugVectorVelocity = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 200, 0));
	//debugVectorPursuit = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 150, 100));
	//debugVectorHeading = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 100, 200));
	//debugVectorSide = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(200, 100, 0));
	//debugVectorAvoid = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 255, 255));
	//debugVectorWander = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(255, 255, 0));
	//debugVectorHide = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(255, 255, 255));
	//debugVectorFlee = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(255, 0, 0));
	//this->addChild(debugVectorVelocity);
	//this->addChild(debugVectorPursuit);
	//this->addChild(debugVectorHeading);
	//this->addChild(debugVectorSide);
	//this->addChild(debugVectorAvoid);
	//this->addChild(debugVectorWander);
	//this->addChild(debugVectorHide);
	//this->addChild(debugVectorFlee);

	//debugVectorCohesion = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(200, 0, 0));
	//debugVectorSeparation = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 200, 0));
	//debugVectorAlignment = std::make_shared<Line>(sf::Vector2f(), sf::Vector2f(), 1.f, sf::Color(0, 0, 200));
	//this->addChild(debugVectorCohesion);
	//this->addChild(debugVectorSeparation);
	//this->addChild(debugVectorAlignment);

	//debugBoundingRect = std::make_shared<BoundingRect>(sf::Vector2f(), sf::Vector2f(), radius, sf::Color(255, 0, 0));
	//this->addChild(debugBoundingRect);
}

void Enemy::onUpdate(double _dt)
{	
	this->updatePosition(_dt);
}

void Enemy::updatePosition(double _dt)
{
	auto acc = this->controller->getSteeringForce() / this->getMass();
	this->velocity += acc * (float)_dt;

	if (fe::math::lengthSquare(this->velocity) > 0.00000001) {
		this->velocity = fe::math::truncate(this->velocity, this->maxSpeed);
		this->setHeading(this->velocity);
	}

	// DEBUG
	//debugVectorVelocity->setPoints(sf::Vector2f(), this->velocity);
	//debugVectorHeading->setPoints(sf::Vector2f(), this->heading * 20.f);
	//debugVectorSide->setPoints(sf::Vector2f(), this->side * 20.f);

	//this->debugBoundingRect->setPoints(sf::Vector2f(), this->heading * 80.f);

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

void Enemy::setAttacking(bool _attacking)
{
	if (_attacking) {
		this->circle->setColor(Enemy::COLOR_ATTACKING);
		this->controller->setAttackMode(_attacking);
	}
	else {
		this->circle->setColor(Enemy::COLOR_BASIC);
		this->controller->setAttackMode(_attacking);
	}
}

void Enemy::setColor(sf::Color _color)
{
	this->circle->setColor(_color);
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
