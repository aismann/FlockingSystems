#include "PlayerUnit.h"

#include "Engine.h"
#include "Shape2D.h"
#include "InputManager.h"
#include "Console.h"
#include "Math.h"
#include "Scene.h"
#include "Ray.h"

#include <SFML/Main.hpp>

#include <memory>

PlayerUnit::PlayerUnit(std::shared_ptr<fe::Scene> _scene, sf::Vector2f _origin) :
	Node2D(_origin),
	hp(100)
{
	// Hook main window
	this->mainWindowPtr = fe::EngineInstance.getMainWindow();
	this->mainScenePtr = _scene;

	// Unit arrow
	auto shape = std::make_shared<sf::CircleShape>(20.f, 3);
	shape->setFillColor(sf::Color(255, 0, 0));
	shape->setOrigin(sf::Vector2f(20.f, 20.f));

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
		auto ray = std::make_shared<Ray>(this->getPosition(), (this->lastMousePoint - this->getPosition()) * 200.f );
		this->mainScenePtr->addChild(ray);
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
	this->setRotation(fe::math::radToDeg(rotRadian) - 30.0);
}

void PlayerUnit::onDraw(sf::RenderTarget& _target)
{
}
