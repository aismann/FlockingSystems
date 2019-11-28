#pragma once
#include "Node2D.h"

class Scene;

class Shape2D :
	public Node2D
{
public:
	/*********** Constructor / Destructor */
	Shape2D(Scene* _scene, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	Shape2D(Node2D* _parent, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~Shape2D();

	/*********** Base */
	virtual void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates()) override;

	/*********** Internal shape */
	void setShape(sf::Shape* _shape);
	sf::Shape* getShape();

private:
	/*********** Internal shape */
	sf::Shape* shape;
};

