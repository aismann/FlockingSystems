#include "MainScene.h"

#include "Engine.h"
#include "InputManager.h"
#include "Shape2D.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Console.h"

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
	auto obstaclePos = std::vector<sf::Vector2f>{
		worldCenter + sf::Vector2f(-150.f, -150.f),
		worldCenter + sf::Vector2f(150.f, -150.f),
		worldCenter + sf::Vector2f(-150.f, 150.f),
		worldCenter + sf::Vector2f(150.f, 150.f)
	};

	for (auto pos : obstaclePos) {
		auto newObstacle = std::make_shared<Obstacle>(pos);
		this->addChild(newObstacle);
		this->obstacles.push_back(newObstacle);
	}

	//this->spawnEnemy(sf::Vector2f(30, 30));

	//for (int i = 0; i < 8; ++i) {
	//	for (int j = 0; j < 8; ++j) {
	//		auto pos = sf::Vector2f(60.f + 100.f * i, 60.f + 100.f * j);
	//		auto newObstacle = std::make_shared<Obstacle>(pos);
	//		this->addChild(newObstacle);
	//		this->obstacles.push_back(newObstacle);
	//	}
	//}

	// Spawn enemies
	const int ENEMIES_NUM = 100;
	for (int i = 0; i < ENEMIES_NUM; ++i) {
		int randX = (float)(rand() % window->getSize().x);
		int randY = (float)(rand() % window->getSize().y);

		this->spawnEnemy(sf::Vector2f(randX, randY));
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

	// Spawn unit on click
	if (_event.type == sf::Event::EventType::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Right) {
		sf::Vector2i mousePos = sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y);
		auto window = fe::EngineInstance.getMainWindow();

		auto enemyPos = window->mapPixelToCoords(mousePos);
		this->spawnEnemy(enemyPos);
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

std::vector<std::weak_ptr<Obstacle>>& MainScene::getObstacles()
{
	return obstacles;
}

std::vector<std::weak_ptr<Enemy>>& MainScene::getEnemies()
{
	return this->enemies;
}

std::weak_ptr<PlayerUnit> MainScene::getPlayerUnit()
{
	return unit;
}

void MainScene::tagEnemiesInRange(sf::Vector2f _start, float _range)
{
	for (auto ptrWeakEnemy : this->enemies) {
		auto ptrEnemy = ptrWeakEnemy.lock();

		if (!ptrEnemy) {
			continue;
		}

		ptrEnemy->setTag(false);

		float distSq = fe::math::lengthSquare(_start - ptrEnemy->getPosition());
		float radius = _range + ptrEnemy->getRadius();
		if (distSq < radius * radius) {
			ptrEnemy->setTag(true);
		}
	}
}

void MainScene::spawnEnemy(sf::Vector2f _pos)
{
	auto ptr = std::static_pointer_cast<MainScene>(shared_from_this());
	auto newEnemy = std::make_shared<Enemy>(ptr, _pos);
	this->enemies.push_back(newEnemy);
	this->addChild(newEnemy);
}
