#include "EnemyController.h"

#include "Console.h"
#include "Math.h"
#include "Enemy.h"
#include "Line.h"
#include "Obstacle.h"
#include "MainScene.h"
#include "PlayerUnit.h"

EnemyController::EnemyController(Enemy* _unit) :
	unit(_unit),
	behaviourFlags(0)
{
}

EnemyController::~EnemyController()
{
}

void EnemyController::onUpdate(double _dt)
{
	this->updateSteeringForce();
}

void EnemyController::updateSteeringForce()
{
	this->steeringForce = sf::Vector2f();
	sf::Vector2f vector;

	if (isBehaviourMode(Behaviour::OBSTACLE_AVOIDANCE)) {
		auto ptr = this->unit->getMainScene().lock();
		if (ptr) {
			auto obstacles = ptr->getObstacles();
			vector = this->avoidObstacles(obstacles);
			unit->debugVectorAvoid->setPoints(sf::Vector2f(), vector);
		}

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::WANDER)) {
		vector = this->wander();
		unit->debugVectorWander->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::HIDE)) {
		auto ptr = this->unit->getMainScene().lock();
		if (ptr) {
			auto obstacles = ptr->getObstacles();
			auto playerUnitPos = ptr->getPlayerUnit().lock()->getPosition();

			vector = this->hide(playerUnitPos, obstacles);
		};

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::PURSUIT)) {
		auto ptr = this->unit->getMainScene().lock();
		if (ptr) {
			auto playerUnit = ptr->getPlayerUnit().lock();
			vector = this->pursuit(playerUnit);
		}

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	// Flocking

}

bool EnemyController::addForce(sf::Vector2f& _currForce, sf::Vector2f _addForce)
{
	float magnitudeToUse = unit->getMaxForce() - fe::math::length(_currForce);

	if (magnitudeToUse <= 0.f) {
		return true; // true if force is full 
	}

	float magnitudeToAdd = fe::math::length(_addForce);
	if (magnitudeToAdd < magnitudeToUse) {
		_currForce += _addForce;
	}
	else {
		_currForce += fe::math::normalize(_addForce) * magnitudeToUse;
	}

	return false; // force is not full
}

sf::Vector2f EnemyController::seek(sf::Vector2f _targetPos)
{
	sf::Vector2f desiredVelocity = fe::math::normalize(_targetPos - unit->getPosition()) * unit->getMaxSpeed();
	return (desiredVelocity - unit->getVelocity());
}

sf::Vector2f EnemyController::flee(sf::Vector2f _targetPos)
{
	const float panicDistSq = 100.0 * 100.0;
	if (fe::math::lengthSquare(unit->getPosition() - _targetPos) > panicDistSq) {
		return sf::Vector2f();
	}

	sf::Vector2f desiredVelocity = fe::math::normalize(unit->getPosition() - _targetPos) * unit->getMaxSpeed();
	return (desiredVelocity - unit->getVelocity());
}

sf::Vector2f EnemyController::arrive(sf::Vector2f _targetPos, Deceleration _dec)
{
	sf::Vector2f toTarget = _targetPos - unit->getPosition();
	float dist = fe::math::length(toTarget);

	if (dist > 0) {
		float speed = dist / ((float)_dec * 0.3f);
		speed = fe::math::min(speed, unit->getMaxSpeed());

		sf::Vector2f desiredVelocity = toTarget * speed / dist;
		return (desiredVelocity - unit->getVelocity());
	}
	
	return sf::Vector2f();
}

sf::Vector2f EnemyController::hide(sf::Vector2f _targetPos, std::vector<std::weak_ptr<Obstacle>>& _obstacles)
{
	sf::Vector2f bestPosition;
	float bestDistToHide = std::numeric_limits<float>::max();

	const float BEHIND = 40.f;

	for (auto ptr : _obstacles) {
		auto obstacle = ptr.lock();

		if (!obstacle) {
			continue;
		}

		auto hidePosition = obstacle->getPosition();
		hidePosition += fe::math::normalize(obstacle->getPosition() - _targetPos) * BEHIND;

		auto distToHide = fe::math::lengthSquare(hidePosition - unit->getPosition());

		if (distToHide < bestDistToHide) {
			bestPosition = hidePosition;
			bestDistToHide = distToHide;
		}
	}

	return arrive(bestPosition, Deceleration::FAST);
}

sf::Vector2f EnemyController::evade(std::shared_ptr<PlayerUnit> _target)
{
	sf::Vector2f toPursuer = _target->getPosition() - this->unit->getPosition();
	float lookAheadTime = fe::math::length(toPursuer) / (this->unit->getMaxSpeed() + _target->getSpeed());

	return flee(_target->getPosition() + _target->getVelocity() * lookAheadTime);
}

sf::Vector2f EnemyController::pursuit(std::shared_ptr<PlayerUnit> _target)
{
	sf::Vector2f toEvader = _target->getPosition() - unit->getPosition();
	float relHeading = fe::math::dotProduct(unit->getHeading(), _target->getHeading());

	if ((fe::math::dotProduct(toEvader, unit->getHeading()) > 0) &&
		(relHeading < -0.95)) // acos(0.95) = 18 degs
	{
		return seek(_target->getPosition());
	}

	float lookAheadTime = fe::math::length(toEvader) / (unit->getMaxSpeed() + _target->getSpeed());
	return seek(_target->getPosition() + _target->getVelocity() * lookAheadTime);
}

sf::Vector2f EnemyController::wander()
{
	wanderTarget += sf::Vector2f(fe::math::randomClamped() * wanderJitter, fe::math::randomClamped() * wanderJitter);
	wanderTarget = fe::math::normalize(wanderTarget);
	wanderTarget *= wanderRadius;

	sf::Vector2f wanderTargetLocal = wanderTarget + sf::Vector2f(wanderDistance, 0);
	sf::Vector2f wanderTargetGlobal = unit->pointToWorldSpace(wanderTargetLocal);

	return (wanderTargetGlobal - unit->getPosition());
}

sf::Vector2f EnemyController::avoidObstacles(std::vector<std::weak_ptr<Obstacle>>& _obstacle)
{
	float boxLength = boudingBoxLength + (unit->getSpeed() / unit->getMaxSpeed()) * boudingBoxLength;

	// Get obstacles in range
	std::shared_ptr<Obstacle>	closestObstacle;
	sf::Vector2f				closestObstacleLocal;
	float						closestObstacleIP = std::numeric_limits<float>::max();

	for (auto ptrWeakObstacle : _obstacle) {
		auto ptrObstacle = ptrWeakObstacle.lock();
		if (!ptrObstacle) {
			continue;
		}

		// Check if in range
		float distSquare = fe::math::lengthSquare(unit->getPosition() - ptrObstacle->getPosition());
		float radiusSquare = fe::math::pow(boudingBoxLength + ptrObstacle->getRadius(), 2);
		if (distSquare > radiusSquare) {
			continue;
		}

		// Convert to local position
		auto localPos = unit->pointToLocalSpace(ptrObstacle->getPosition());
		if (localPos.x < 0) {
			continue;
		}

		float expandedRadius = unit->getRadius() + ptrObstacle->getRadius() + offsetRadius;
		if (expandedRadius < fe::math::abs(localPos.y)) {
			continue;
		}

		// Get closest intersection point
		float sqrtPart = fe::math::sqrt(expandedRadius * expandedRadius - localPos.y * localPos.y);
		float ip = ((localPos.x - sqrtPart < 0) ? (localPos.x + sqrtPart) : (localPos.x - sqrtPart));

		if (ip < closestObstacleIP) {
			closestObstacleIP = ip;
			closestObstacle = ptrObstacle;
			closestObstacleLocal = localPos;
		}
	}

	// Obstacle not found
	if (!closestObstacle) {
		return sf::Vector2f();
	}

	// Calculate steering force
	float multiplier = 1.f + (boudingBoxLength - closestObstacleLocal.x) / boudingBoxLength;
	//auto lateral = (2.0f - (fe::math::abs(closestObstacleLocal.y) / closestObstacle->getRadius())) * (-fe::math::sign(closestObstacleLocal.y)) * closestObstacle->getRadius() * multiplier;
	//auto lateral = (closestObstacle->getRadius() - closestObstacleLocal.y) * multiplier;
	auto lateral = -(fe::math::pow(closestObstacle->getRadius(),2)/closestObstacleLocal.y) * multiplier;
	auto steeringForce = sf::Vector2f(0.f, lateral);

	const float brakingWeight = 0.2f;
	steeringForce.x = (closestObstacle->getRadius() - closestObstacleLocal.x) * brakingWeight;

	return unit->vectorToWorldSpace(steeringForce);
}

sf::Vector2f EnemyController::separation(std::vector<std::weak_ptr<Enemy>>& _enemies)
{
	sf::Vector2f tmpSteeringForce;

	for (auto unitWeakPtr : _enemies) {
		auto unitPtr = unitWeakPtr.lock();
		if (!unitPtr || !unitPtr->isTagged()) {
			continue;
		}

		sf::Vector2f toNode = unit->getPosition() - unitPtr->getPosition();
		tmpSteeringForce += fe::math::normalize(toNode) / fe::math::length(toNode);
	}

	return tmpSteeringForce;
}

sf::Vector2f EnemyController::alignment(std::vector<std::weak_ptr<Enemy>>& _enemies)
{
	sf::Vector2f	averageHeading;
	int				neighborCount = 0;

	for (auto unitWeakPtr : _enemies) {
		auto unitPtr = unitWeakPtr.lock();
		if (!unitPtr || !unitPtr->isTagged()) {
			continue;
		}

		averageHeading += unitPtr->getHeading();
		neighborCount++;
	}

	if (neighborCount > 0) {
		averageHeading /= (float)neighborCount;
		averageHeading -= unit->getHeading();
	}

	return averageHeading;
}

sf::Vector2f EnemyController::cohesion(std::vector<std::weak_ptr<Enemy>>& _enemies)
{
	sf::Vector2f centerOfMass, tmpSteeringForce;
	int neighborCount = 0;

	for (auto unitWeakPtr : _enemies) {
		auto unitPtr = unitWeakPtr.lock();
		if (!unitPtr || !unitPtr->isTagged()) {
			continue;
		}

		centerOfMass += unitPtr->getPosition();
		neighborCount++;
	}

	if (neighborCount > 0) {
		centerOfMass /= (float)neighborCount;
		tmpSteeringForce = seek(centerOfMass);
	}

	return tmpSteeringForce;
}
