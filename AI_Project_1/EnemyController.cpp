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
	untilHidingTimer = 2.f + (float)(rand() % 4);
}

EnemyController::~EnemyController()
{
}

void EnemyController::onUpdate(double _dt)
{
	// Attack timer
	if (this->attackMode) {
		if (this->hitTimer >= 0.0) {
			this->hitTimer -= _dt;
		}
		else {
			this->hitTimer = HIT_CHECK;

			auto scene = this->unit->getMainScene().lock();
			if (!scene) { throw std::exception("Cannot get main scene pointer."); }

			scene->tryHitPlayer(this->unit, this->hitRadius);
		}
	}
	
	// Group timer
	if (this->groupTimer >= 0.0) {
		groupTimer -= _dt;
	}

	// Hide by choice timer
	if (!attackMode) {
		if (!this->isHiding) {
			if (this->untilHidingTimer >= 0.0) {
				untilHidingTimer -= _dt;
			}
			else {
				isHiding = true;
				unit->setColor(sf::Color(0, 0, 200));

				hidingTimer = 5.f + (float)(rand() % 5);
			}
		}
		else {
			if (this->hidingTimer >= 0.0) {
				hidingTimer -= _dt;
			}
			else {
				isHiding = false;
				unit->setColor(Enemy::COLOR_BASIC);

				hidingSpot.reset();
				untilHidingTimer = 2.f + (float)(rand() % 6);
			}
		}
	}

	this->updateSteeringForce();
}

void EnemyController::updateSteeringForce()
{
	// Zero force
	this->steeringForce = sf::Vector2f();
	sf::Vector2f vector;

	// Get data
	auto scene = this->unit->getMainScene().lock();
	if (!scene) { throw std::exception("Cannot get main scene pointer."); }

	auto& enemies = scene->getEnemies();
	auto& obstacles = scene->getObstacles();
	auto playerUnit = scene->getPlayerUnit().lock();
	if (!playerUnit) { throw std::exception("Cannot get player unit pointer."); }

	// Random hiding point
	if (isHiding && !hidingSpot && (obstacles.size() > 0)) {
		int iObstacle = rand() % obstacles.size();
		hidingSpot = obstacles[iObstacle];
	}

	// Behaviours
	if (isBehaviourMode(Behaviour::OBSTACLE_AVOIDANCE)) {
		vector = this->avoidObstacles(obstacles) * weightAvoidance;
		//unit->debugVectorAvoid->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::PURSUIT) && attackMode) {
		vector = this->pursuit(playerUnit) * weightPursuit;

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::FLEE) && !attackMode) {
		auto playerUnitPos = playerUnit->getPosition();
		vector = this->flee(playerUnitPos) * weightFlee;
		//unit->debugVectorFlee->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	// force hide
	if (isBehaviourMode(Behaviour::FORCE_HIDE) && !attackMode && isHiding && hidingSpot) {
		auto playerUnitPos = playerUnit->getPosition();
		vector = this->hide_forced(playerUnitPos) * weightHide;
		//unit->debugVectorHide->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::HIDE) && !attackMode && !isHiding) {
		auto playerUnitPos = playerUnit->getPosition();
		vector = this->hide(playerUnitPos, obstacles) * weightHide;
		//unit->debugVectorHide->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::WANDER) && !this->attackMode) {
		vector = this->wander() * weightWander;
		//unit->debugVectorWander->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	// Check enemies in range
	int count = scene->tagEnemiesInRange(this->unit, this->viewDistance);

	// Form attacking group
	if (count >= this->groupCount && !this->attackMode) {
		for (auto& enemy : enemies) {
			if (enemy->isTagged()) {
				enemy->setAttacking(true);
			}
		}

		unit->setAttacking(true);
	}

	// Flocking
	if (isBehaviourMode(Behaviour::SEPARATION)) {
		vector = this->separation(enemies) * weightSeparation;
		//unit->debugVectorSeparation->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::ALLIGNMENT)) {
		vector = this->alignment(enemies) * weightAlignment;
		//unit->debugVectorAlignment->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}

	if (isBehaviourMode(Behaviour::COHESION)) {
		vector = this->cohesion(enemies) * weightCohesion;
		//unit->debugVectorCohesion->setPoints(sf::Vector2f(), vector);

		if (addForce(steeringForce, vector)) {
			return;
		}
	}
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
		return false; // force is not full
	}
	else {
		_currForce += fe::math::normalize(_addForce) * magnitudeToUse;
		return true; // force is full
	}
}

void EnemyController::setAttackMode(bool _attacking)
{
	this->attackMode = _attacking;
}

sf::Vector2f EnemyController::seek(sf::Vector2f _targetPos)
{
	auto len = fe::math::lengthSquare(_targetPos - unit->getPosition());
	if (len < 0.0001) {
		return sf::Vector2f();
	}

	sf::Vector2f desiredVelocity = fe::math::normalize(_targetPos - unit->getPosition()) * unit->getMaxSpeed();
	return (desiredVelocity - unit->getVelocity());
}

sf::Vector2f EnemyController::flee(sf::Vector2f _targetPos)
{
	if (fe::math::lengthSquare(unit->getPosition() - _targetPos) > fleeDistance * fleeDistance) {
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

sf::Vector2f EnemyController::hide(sf::Vector2f _targetPos, std::vector<std::shared_ptr<Obstacle>>& _obstacles)
{
	sf::Vector2f bestPosition;
	float bestDistToHide = std::numeric_limits<float>::max();

	// check if in hide radius
	if (fe::math::lengthSquare(_targetPos - unit->getPosition()) > (hideDistance * hideDistance)) {
		return sf::Vector2f();
	}

	// find obstacle
	for (auto obstacle : _obstacles) {
		if (!obstacle) {
			continue;
		}

		auto hidePosition = obstacle->getPosition();
		hidePosition += fe::math::normalize(obstacle->getPosition() - _targetPos) * obstacleOffset;

		auto distToHide = fe::math::lengthSquare(hidePosition - unit->getPosition());

		if (distToHide < bestDistToHide) {
			bestPosition = hidePosition;
			bestDistToHide = distToHide;
		}
	}

	return arrive(bestPosition, Deceleration::FAST);
}

sf::Vector2f EnemyController::hide_forced(sf::Vector2f _targetPos)
{	
	auto hidePosition = hidingSpot->getPosition();
	hidePosition += fe::math::normalize(hidingSpot->getPosition() - _targetPos) * obstacleOffset;

	return arrive(hidePosition, Deceleration::FAST);
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

sf::Vector2f EnemyController::avoidObstacles(std::vector<std::shared_ptr<Obstacle>>& _obstacle)
{
	float boxLength = boudingBoxLength + (unit->getSpeed() / unit->getMaxSpeed()) * boudingBoxLength;

	// Get obstacles in range
	std::shared_ptr<Obstacle>	closestObstacle;
	sf::Vector2f				closestObstacleLocal;
	float						closestObstacleIP = std::numeric_limits<float>::max();

	for (auto ptrObstacle : _obstacle) {
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
	//auto lateral = (closestObstacle->getRadius() - closestObstacleLocal.y) * multiplier; // from book
	auto lateral = -(fe::math::pow(closestObstacle->getRadius(),2)/closestObstacleLocal.y) * multiplier;
	auto steeringForce = sf::Vector2f(0.f, lateral);

	const float brakingWeight = 0.2f;
	steeringForce.x = (closestObstacle->getRadius() - closestObstacleLocal.x) * brakingWeight;

	return unit->vectorToWorldSpace(steeringForce);
}

sf::Vector2f EnemyController::separation(std::vector<std::shared_ptr<Enemy>>& _enemies)
{
	sf::Vector2f tmpSteeringForce;
	int count = 0;

	for (auto unitPtr : _enemies) {
		if (!unitPtr->isTagged()) {
			continue;
		}

		auto toAgent = (unit->getPosition() - unitPtr->getPosition());
		auto len = fe::math::length(toAgent);

		if (len < 25.f) {
			tmpSteeringForce += fe::math::normalize(toAgent) / len;
			count++;
		}
	}

	if (count > 0) {
		// tu sie dzieja rozne rzeczy z ktorymi mozna poeksperymentowac
		tmpSteeringForce /= (float)count;
		tmpSteeringForce = fe::math::normalize(tmpSteeringForce);
		tmpSteeringForce *= unit->getMaxSpeed();
		tmpSteeringForce -= unit->getVelocity();
		return tmpSteeringForce;
	}
	else {
		return sf::Vector2f();
	}
}

sf::Vector2f EnemyController::alignment(std::vector<std::shared_ptr<Enemy>>& _enemies)
{
	sf::Vector2f averageHeading;
	int count = 0;

	for (auto unitPtr : _enemies) {
		if (!unitPtr->isTagged()) {
			continue;
		}

		averageHeading += unitPtr->getHeading();
		count++;
	}

	if (count > 0) {
		averageHeading /= (float)count;
		return averageHeading;
	}
	else {
		return sf::Vector2f();
	}
}

sf::Vector2f EnemyController::cohesion(std::vector<std::shared_ptr<Enemy>>& _enemies)
{
	sf::Vector2f tmpSteeringForce;
	int count = 0;

	for (auto unitPtr : _enemies) {
		if (!unitPtr || !unitPtr->isTagged()) {
			continue;
		}

		tmpSteeringForce += unitPtr->getPosition();
		count++;
	}
	
	if (count > 0) {
		tmpSteeringForce /= (float)count;
		tmpSteeringForce = tmpSteeringForce - unit->getPosition();
		tmpSteeringForce = fe::math::normalize(tmpSteeringForce);
		return tmpSteeringForce;
	}
	else {
		return sf::Vector2f();
	}
}
