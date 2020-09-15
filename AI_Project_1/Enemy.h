#pragma once
#include <SFML/System.hpp>

#include "PhysicNode.h"

#include <map>

class EnemyController;
class MainScene;
class Line;
class BoundingRect;

namespace fe {
	class Shape2D;
}

class Enemy:
	public fe::PhysicNode
{
public:
	friend EnemyController;

	/*********** Constructor / Destructor */
			Enemy(std::shared_ptr<MainScene> _scene, sf::Vector2f _position = sf::Vector2f());
	virtual ~Enemy();

	/*********** Base */
	void onInit() override;

	void onUpdate(double _dt) override;
	void onDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates()) override;

	/*********** Game logic */
	void						rayHit();
	void						setAttacking(bool _attacking);
	void						setColor(sf::Color _color);

	void						setTag(bool _tag);
	bool						isTagged();

	std::weak_ptr<MainScene>	getMainScene() { return this->mainScene; };
	float						getRadius() { return this->radius; };

protected:
	/*********** Movement */
	void						updatePosition(double _dt);
	void						wrapScreen();

protected:
	/*********** Components */
	static const sf::Color COLOR_BASIC;
	static const sf::Color COLOR_ATTACKING;

	std::shared_ptr<fe::Shape2D> circle;

	/*********** Settings */
	sf::Vector2u		worldSizeEnd;	// size of a visible world
	float				radius;			// half size of a enemy

	bool				tagged;

	/*********** AI */
	std::shared_ptr<EnemyController>	controller;
	std::weak_ptr<MainScene>			mainScene;

	/*********** Debug */
	//std::shared_ptr<BoundingRect>	debugBoundingRect;
	//std::shared_ptr<Line>			debugVectorVelocity;
	//std::shared_ptr<Line>			debugVectorPursuit;
	//std::shared_ptr<Line>			debugVectorHeading;
	//std::shared_ptr<Line>			debugVectorSide;
	//std::shared_ptr<Line>			debugVectorAvoid;
	//std::shared_ptr<Line>			debugVectorWander;
	//std::shared_ptr<Line>			debugVectorHide;
	//std::shared_ptr<Line>			debugVectorFlee;

	//std::shared_ptr<Line>			debugVectorCohesion;
	//std::shared_ptr<Line>			debugVectorSeparation;
	//std::shared_ptr<Line>			debugVectorAlignment;
};
