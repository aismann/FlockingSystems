#pragma once
#include "Node2D.h"

namespace fe {
	class Shape2D;
	class Scene;
}

class PlayerUnit:
	public fe::Node2D
{
public:
	/*********** Constructor / Destructor */
	PlayerUnit(std::shared_ptr<fe::Scene> _scene, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~PlayerUnit();

	/*********** To implement in game */
	virtual void onEvent(sf::Event& _event);
	virtual void onUpdate(double _dt);
	virtual void onDraw(sf::RenderTarget& _target);

private:
	/*********** Components */
	// Hooks
	std::shared_ptr<sf::RenderWindow>	mainWindowPtr;
	std::shared_ptr<fe::Scene>			mainScenePtr;

	std::shared_ptr<fe::Shape2D>		shapeShip;

	/*********** Movement */
	sf::Vector2f lastMousePoint;

	/*********** Settings */
	int		hp;
	float	speed = 300.f;
};

