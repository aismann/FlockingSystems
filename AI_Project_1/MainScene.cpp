#include "MainScene.h"

#include "Engine.h"
#include "InputManager.h"
#include "Shape2D.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Console.h"

#include <cstdlib>
#include "ProgressBar.h""

MainScene::MainScene():
	unit(nullptr)
{
}

MainScene::~MainScene()
{
}

void MainScene::onInit()
{
	window = fe::EngineInstance.getMainWindow();
	auto worldCenter = window->mapPixelToCoords(sf::Vector2i(window->getSize())) / 2.f;

	/*** Spawn player ***/
	unit = std::make_shared<PlayerUnit>(shared_from_this());
	unit->setPosition(worldCenter);
	this->addChild(unit);

	/*** Spawn obstacles ***/
	//this->spawnObstaclesRandom();
	//this->spawnObstaclesNarrow();
	this->spawnObstaclesFour();

	/*** Spawn enemies ***/
	const int ENEMIES_NUM = 20;
	for (int i = 0; i < ENEMIES_NUM; ++i) {
		int randX = (float)(rand() % window->getSize().x);
		int randY = (float)(rand() % window->getSize().y);

		this->spawnEnemy(sf::Vector2f(randX, randY));
	}

	/*** Show HP bar ***/
	auto hp_bar = std::make_shared<ProgressBar>(sf::Vector2f(20.f, 40.f), 200.f);
	unit->addHpBar(hp_bar);
	this->addChild(hp_bar);
}

void MainScene::onExit()
{
}

void MainScene::onEvent(sf::Event& _event)
{
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
	// Spawner
	if (spawnTimer >= 0.0) {
		spawnTimer -= _dt;
	}
	else {
		this->spawnEnemyRandom();
		spawnTimer = SPAWN_DELTA;
	}

	// Update enemies
	for (int i = 0; i < enemies.size(); /* conditional */) {
		auto child = enemies[i];

		if (child->isDisabled()) {
			continue;
		}

		child->onBaseUpdate(_dt);

		if (child->isDeleted()) {
			int lastIt = enemies.size() - 1;
			std::swap(enemies[i], enemies[lastIt]);

			child->onExit();
			enemies.pop_back();
		}
		else {
			i++;
		}
	}
}

void MainScene::onDraw(sf::RenderTarget& _target)
{
	// Update enemies
	for (auto enemy : enemies) {
		if (enemy->isDisabled()) {
			continue;
		}

		enemy->onBaseDraw(_target);
	}
}

std::vector<std::shared_ptr<Obstacle>>& MainScene::getObstacles()
{
	return obstacles;
}

std::vector<std::shared_ptr<Enemy>>& MainScene::getEnemies()
{
	return this->enemies;
}

std::weak_ptr<PlayerUnit> MainScene::getPlayerUnit()
{
	return unit;
}

int MainScene::tagEnemiesInRange(Enemy* _unit, float _range)
{
	sf::Vector2f start = _unit->getPosition();
	int count = 0;

	for (auto enemy : this->enemies) {
		enemy->setTag(false);

		if (enemy.get() == _unit) {
			continue;
		}

		float distSq = fe::math::lengthSquare(start - enemy->getPosition());
		float radius = _range + enemy->getRadius();
		if (distSq < radius * radius) {
			enemy->setTag(true);
			count++;
		}
	}

	return count;
}

bool MainScene::tryHitPlayer(Enemy* _unit, float _range)
{
	sf::Vector2f start = _unit->getPosition();
	float distSq = fe::math::lengthSquare(start - unit->getPosition());

	if (distSq < _range * _range) {
		unit->onHit();
		return true;
	}

	return false;
}

void MainScene::spawnEnemy(sf::Vector2f _pos)
{
	auto ptr = std::static_pointer_cast<MainScene>(shared_from_this());

	auto newEnemy = std::make_shared<Enemy>(ptr, _pos);
	newEnemy->onInit();
	
	this->enemies.push_back(newEnemy);
}

void MainScene::spawnEnemyRandom()
{
	int randX = (float)(rand() % window->getSize().x);
	int randY = (float)(rand() % window->getSize().y);

	this->spawnEnemy(sf::Vector2f(randX, randY));
}

void MainScene::spawnObstaclesFour()
{
	auto worldCenter = window->mapPixelToCoords(sf::Vector2i(window->getSize())) / 2.f;

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
}

void MainScene::spawnObstaclesNarrow()
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			auto pos = sf::Vector2f(60.f + 100.f * i, 60.f + 100.f * j);
			auto newObstacle = std::make_shared<Obstacle>(pos);
			this->addChild(newObstacle);
			this->obstacles.push_back(newObstacle);
		}
	}
}

void MainScene::spawnObstaclesRandom()
{
	for (int i = 0; i < 15; ++i) {
		int randX = (float)(rand() % window->getSize().x);
		int randY = (float)(rand() % window->getSize().y);

		auto pos = sf::Vector2f(randX, randY);
		auto newObstacle = std::make_shared<Obstacle>(pos);

		this->addChild(newObstacle);
		this->obstacles.push_back(newObstacle);
	}
}
