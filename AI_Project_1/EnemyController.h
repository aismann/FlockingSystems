#pragma once
#include "Node2D.h"

class Enemy;
class Obstacle;
class PlayerUnit;

class EnemyController:
	public fe::Node
{
public:
	enum Deceleration {
		SLOW	= 3,
		NORMAL	= 2,
		FAST	= 1
	};

	enum Behaviour {
		NONE				= 0,
		SEEK				= 1,
		FLEE				= 1 << 1,
		ARRIVE				= 1 << 2,
		WANDER				= 1 << 3,
		COHESION			= 1 << 4,
		SEPARATION			= 1 << 5,
		ALLIGNMENT			= 1 << 6,
		OBSTACLE_AVOIDANCE	= 1 << 7,
		WALL_AVOIDANCE		= 1 << 8,
		FOLLOW_PATH			= 1 << 9,
		PURSUIT				= 1 << 10,
		EVADE				= 1 << 11,
		INTERPOSE			= 1 << 12,
		HIDE				= 1 << 13,
		FLOCK				= 1 << 14,
		OFFSET_PURSUIT		= 1 << 15,
		FORCE_HIDE			= 1 << 16
	};

	/*********** Constructor / Destructor */
	EnemyController(Enemy* _unit);
	virtual ~EnemyController();

	/*********** To implement in game */
	void onUpdate(double _dt) override;

	/*********** AI Behaviors */
	void updateSteeringForce();
	sf::Vector2f getSteeringForce() { return this->steeringForce; };

	void		setAttackMode(bool _attacking);

	/*********** AI Behaviors Flags */
	void removeBehaviourMode(Behaviour _flag) { this->behaviourFlags &= (~_flag); };
	void addBehaviourMode(Behaviour _flag) { this->behaviourFlags |= _flag;};
	bool isBehaviourMode(Behaviour _flag) { return (this->behaviourFlags & _flag); };

private:
	/*********** AI Behaviors */
	bool		addForce(sf::Vector2f& _currForce, sf::Vector2f _addForce);

	/*********** AI Behaviors */
	sf::Vector2f seek(sf::Vector2f _targetPos);
	sf::Vector2f flee(sf::Vector2f _targetPos);
	sf::Vector2f arrive(sf::Vector2f _targetPos, Deceleration _dec);	

	sf::Vector2f evade(std::shared_ptr<PlayerUnit> _target);
	sf::Vector2f pursuit(std::shared_ptr<PlayerUnit> _targetPos);

	sf::Vector2f hide(sf::Vector2f _targetPos, std::vector<std::shared_ptr<Obstacle>>& _obstacles);
	sf::Vector2f hide_forced(sf::Vector2f _targetPos);
	sf::Vector2f wander();

	sf::Vector2f avoidObstacles(std::vector<std::shared_ptr<Obstacle>>& _obstacles);

	// Flocking
	sf::Vector2f separation(std::vector<std::shared_ptr<Enemy>>& _enemies);
	sf::Vector2f alignment(std::vector<std::shared_ptr<Enemy>>& _enemies);
	sf::Vector2f cohesion(std::vector<std::shared_ptr<Enemy>>& _enemies);


private:
	/*********** AI Behaviors */
	int				behaviourFlags;
	sf::Vector2f	steeringForce;

	// Weights
	float weightSeparation	= 10.f;
	float weightAlignment	= 1.f;
	float weightCohesion	= 5.f;

	float weightAvoidance	= 8.f;
	float weightPursuit		= 6.f;
	float weightWander		= 1.f;
	float weightHide		= 3.f;
	float weightFlee		= 4.f;

	// Wander settings
	sf::Vector2f wanderTarget = sf::Vector2f();

	float wanderJitter		= 4.0f;
	float wanderRadius		= 20.f;
	float wanderDistance	= 30.f;

	// Obstacle avoidance settings
	float boudingBoxLength	= 40.f;
	float offsetRadius		= 20.f; // additional offset between Obstacle and Unit while avoidance

	// Flocking behaviour settings
	float viewDistance		= 50.f;
	
	// Flee
	float fleeDistance		= 100.f;

	// Hide settings
	float hideDistance		= 250.f;
	float obstacleOffset	= 60.f; // offset between Obstacle and Unit while hiding

	/*********** Others */
	// Group and attack
	bool attackMode			= false;
	int groupCount			= 5;
	float groupRadius		= 50.f;
	float hitRadius			= 20.f;

	const float GROUP_CHECK = 5.f; // check for grouping interval
	float groupTimer		= GROUP_CHECK;

	const float				HIT_CHECK = 1.f; // check if player hit interval
	float					hitTimer = HIT_CHECK;

	// Choose hiding spot and hide for some time, this allow grouping of enemies
	bool isHiding			= false; // is unit hiding by choice
	float untilHidingTimer	= 0.f;	// timer until unit decide to hide
	float hidingTimer		= 0.f;	// hide for some time then return to normal state
	std::shared_ptr<Obstacle> hidingSpot;

	/*********** Controlled unit */
	Enemy*			unit;
};
