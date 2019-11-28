#include "PlayerUnit.h"
#include "Engine.h"
#include "Shape2D.h"

#include <SFML/Main.hpp>

PlayerUnit::PlayerUnit(Scene* _scene, sf::Vector2f _origin) :
	Node2D(_scene, _origin),
	hp(100)
{
	// Unit arrow
	auto shape = new sf::CircleShape(20.f, 3);
	shape->setFillColor(sf::Color(255, 255, 0));
	shape->setOrigin(sf::Vector2f(20.f, 20.f));

	ship = new Shape2D(this);
	ship->setShape(shape);

	// Physics
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	body = WorldInstance->CreateBody(&bodyDef);
}

PlayerUnit::PlayerUnit(Node2D* _parent, sf::Vector2f _origin):
	Node2D(_parent, _origin),
	hp(100),
	ship(nullptr),
	body(nullptr)
{
}

PlayerUnit::~PlayerUnit()
{
}

void PlayerUnit::onEvent(sf::Event& _event)
{
}

void PlayerUnit::onUpdate(double _dt)
{
	auto& input = InputManagerInstance;

	// Move
	float h = (int)input.isKeyPressed(sf::Keyboard::D) - (int)input.isKeyPressed(sf::Keyboard::A);
	float w = (int)input.isKeyPressed(sf::Keyboard::S) - (int)input.isKeyPressed(sf::Keyboard::W);

	h *= speed * _dt;
	w *= speed * _dt;

	// Sync with physics
	b2Vec2 pos = body->GetPosition();
	this->setPosition(sf::Vector2f(pos.x, -pos.y));
}

void PlayerUnit::onDraw(sf::RenderTarget& _target)
{
	// Draw center
	sf::CircleShape point(2);
	point.setOrigin(sf::Vector2f(1.f,1.f));
	point.setFillColor(sf::Color(100, 250, 50));
	point.setPosition(getPosition());
	_target.draw(point);
}
