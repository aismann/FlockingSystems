#include "PlayerUnit.h"

#include "Engine.h"
#include "Shape2D.h"
#include "InputManager.h"
#include "PhysicsEngine.h"
#include "Console.h"
#include "Math.h"
#include "Scene.h"
#include "Ray.h"
#include "Enemy.h"

#include <SFML/Main.hpp>

#include <memory>

PlayerUnit::PlayerUnit(std::shared_ptr<fe::Scene> _scene, sf::Vector2f _origin) :
	PhysicNode(_origin),
	hp(100)
{
	// Get visible world size
	auto window = fe::EngineInstance.getMainWindow();
	worldSizeEnd = window->getSize();

	// Hook main window
	this->mainWindowPtr = fe::EngineInstance.getMainWindow();
	this->mainScenePtr = _scene;
	
	// Unit heading ray
	auto rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(30.f, 2.f));
	rect->setFillColor(sf::Color(255, 0, 0));

	auto heading = std::make_shared<fe::Shape2D>();
	heading->setShape(rect);
	this->addChild(heading);

	// Unit arrow
	auto shape = std::make_shared<sf::CircleShape>(10.f, 3);
	shape->setFillColor(sf::Color(255, 100, 50));
	shape->setOrigin(sf::Vector2f(10.f, 10.f));
	shape->setScale(sf::Vector2f(1.f, 2.f));

	shapeShip = std::make_shared<fe::Shape2D>();
	shapeShip->setShape(shape);
	this->addChild(shapeShip);
}

PlayerUnit::~PlayerUnit()
{
}

void PlayerUnit::onEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::EventType::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left) {
		// Spawn ray
		auto dir = (this->lastMousePoint - this->getPosition());
		auto ray = std::make_shared<Ray>(this->getPosition(), dir * 200.f );
		this->mainScenePtr->addChild(ray);

		// Get intersect bodies
		auto bodies = fe::EngineInstance.getPhysicEngine()->raycast(this->getPosition(), dir);
		for (auto body : bodies) {
			if (auto ptr = std::dynamic_pointer_cast<Enemy>(body)) {
				ptr->rayHit();
			}
		}
	}

	if (_event.type == sf::Event::EventType::MouseMoved) {
		// Set last mouse position
		auto pixelPos = sf::Vector2i(_event.mouseMove.x, _event.mouseMove.y);
		this->lastMousePoint = this->mainWindowPtr->mapPixelToCoords(pixelPos);
	}
}

void PlayerUnit::onUpdate(double _dt)
{
	auto input = fe::EngineInstance.getInputManager();

	// Move
	float h = (int)input->isKeyPressed(sf::Keyboard::D) - (int)input->isKeyPressed(sf::Keyboard::A);
	float w = (int)input->isKeyPressed(sf::Keyboard::S) - (int)input->isKeyPressed(sf::Keyboard::W);

	h *= speed * _dt;
	w *= speed * _dt;

	this->move(sf::Vector2f(h, w));

	// Rotate
	sf::Vector2f toMouse = this->lastMousePoint - this->getPosition();
	float rotRadian = fe::math::atan2(toMouse.y, toMouse.x);
	this->setRotation(fe::math::radToDeg(rotRadian) + 90.0);

	// Wrap
	this->wrapScreen();
}

void PlayerUnit::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
}

void PlayerUnit::wrapScreen()
{
	auto pos = this->getPosition();

	if (pos.x < 0.f) { pos.x += worldSizeEnd.x; }
	if (pos.x > worldSizeEnd.x) { pos.x -= worldSizeEnd.x; }
	if (pos.y < 0.f) { pos.y += worldSizeEnd.y; }
	if (pos.y > worldSizeEnd.y) { pos.y -= worldSizeEnd.y; }

	this->setPosition(pos);
}

