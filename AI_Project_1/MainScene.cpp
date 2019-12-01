#include "MainScene.h"

#include "Engine.h"
#include "InputManager.h"
#include "Shape2D.h"
#include "Obstacle.h"
#include "Enemy.h"

#include <cstdlib>

MainScene::MainScene():
	unit(nullptr)
{
}

MainScene::~MainScene()
{
}

void MainScene::onInit()
{
	auto window = fe::EngineInstance.getMainWindow();
	auto worldCenter = window->mapPixelToCoords(sf::Vector2i(window->getSize())) / 2.f;

	// Spawn player
	unit = std::make_shared<PlayerUnit>(shared_from_this());
	unit->setPosition(worldCenter);
	this->addChild(unit);

	// Spawn obstacles
	this->addChild(std::make_shared<Obstacle>(worldCenter + sf::Vector2f(-150.f, -150.f)));
	this->addChild(std::make_shared<Obstacle>(worldCenter + sf::Vector2f(150.f, -150.f)));
	this->addChild(std::make_shared<Obstacle>(worldCenter + sf::Vector2f(-150.f, 150.f)));
	this->addChild(std::make_shared<Obstacle>(worldCenter + sf::Vector2f(150.f, 150.f)));

	// Spawn enemies
	const int ENEMIES_NUM = 100;
	for (int i = 0; i < ENEMIES_NUM; ++i) {
		int randX = rand() % window->getSize().x;
		int randY = rand() % window->getSize().y;

		auto newEnemy = std::make_shared<Enemy>(sf::Vector2f(randX, randY));
		this->addChild(newEnemy);
	}
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
