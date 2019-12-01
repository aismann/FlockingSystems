#include "MainScene.h"

#include "Engine.h"
#include "InputManager.h"
#include "Shape2D.h"
#include "Obstacle.h"
#include "Enemy.h"

MainScene::MainScene():
	unit(nullptr)
{
}

MainScene::~MainScene()
{
}

void MainScene::onInit()
{
	// Spawn player
	unit = std::make_shared<PlayerUnit>(shared_from_this());
	this->addChild(unit);

	// Spawn obstacles
	this->addChild(std::make_shared<Obstacle>(sf::Vector2f(100.f, 100.f)));
	this->addChild(std::make_shared<Obstacle>(sf::Vector2f(400.f, 100.f)));
	this->addChild(std::make_shared<Obstacle>(sf::Vector2f(100.f, 400.f)));
	this->addChild(std::make_shared<Obstacle>(sf::Vector2f(400.f, 400.f)));

	// Spawn enemies
	auto newEnemy = std::make_shared<Enemy>(sf::Vector2f(200.f, 200.f));
	this->addChild(newEnemy);
}

void MainScene::onExit()
{
}

void MainScene::onEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::A) {
		//
	}
}

void MainScene::onUpdate(double _dt)
{
	if (fe::EngineInstance.getInputManager()->isKeyPressed(sf::Keyboard::S)) {
		//
	}
}

void MainScene::onDraw(sf::RenderTarget& _target)
{

}
