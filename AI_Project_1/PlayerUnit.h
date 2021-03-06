#pragma once
#include "PhysicNode.h"

namespace fe {
	class Shape2D;
	class Scene;

	struct Contact;
}

class ProgressBar;

class PlayerUnit:
	public fe::PhysicNode
{
public:
	/*********** Constructor / Destructor */
	PlayerUnit(std::shared_ptr<fe::Scene> _scene, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~PlayerUnit();

	/*********** To implement in game */
	void onEvent(sf::Event& _event) override;
	void onUpdate(double _dt) override;
	void onDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates()) override;

	void onInit() override;
	void onBeginContact(fe::Contact& _contact) override;

	// Attach hp bar
	void addHpBar(std::shared_ptr<ProgressBar> _bar);

	void onHit();

private:
	/*********** Movement */
	void wrapScreen();

private:
	/*********** Components */
	// Hooks
	std::shared_ptr<sf::RenderWindow>	mainWindowPtr;
	std::shared_ptr<fe::Scene>			mainScenePtr;

	std::shared_ptr<fe::Shape2D>		shapeShip;
	std::shared_ptr<ProgressBar>		hpBar;

	/*********** Movement */
	sf::Vector2f lastMousePoint;
	sf::Vector2u worldSizeEnd;

	/*********** Settings */
	int		max_hp = 100;
	int		hp = max_hp;
	float	speed = 300.f;
};

