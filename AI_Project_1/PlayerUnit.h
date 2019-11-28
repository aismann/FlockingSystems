#pragma once
#include "Node2D.h"
#include <Box2D/Box2D.h>

class Shape2D;

class PlayerUnit:
	public Node2D
{
public:
	/*********** Constructor / Destructor */
	PlayerUnit(Scene* _scene, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	PlayerUnit(Node2D* _parent, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~PlayerUnit();

	/*********** To implement in game */
	virtual void onEvent(sf::Event& _event);
	virtual void onUpdate(double _dt);
	virtual void onDraw(sf::RenderTarget& _target);

private:
	/*********** Constructor / Destructor */
	Shape2D* ship;

	b2Body* body;

	int hp;
	float speed = 100.f;
};

