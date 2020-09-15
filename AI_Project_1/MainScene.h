#pragma once
#include "Scene.h"
#include "PlayerUnit.h"

class Obstacle;
class Enemy;

class MainScene : 
	public fe::Scene
{
public:
	/*********** Constructor / Destructor */
	MainScene();
	virtual ~MainScene();

	/*********** Base */
	void onInit() override;
	void onExit() override;

	void onEvent(sf::Event& _event) override;
	void onUpdate(double _dt) override;
	void onDraw(sf::RenderTarget& _target) override;

	/*********** AI */
	std::vector<std::shared_ptr<Obstacle>>&	getObstacles();
	std::vector<std::shared_ptr<Enemy>>&	getEnemies();
	std::weak_ptr<PlayerUnit>				getPlayerUnit();

	int										tagEnemiesInRange(Enemy* _unit, float _range);
	bool									tryHitPlayer(Enemy* _unit, float _range);

private:
	/*********** Units */
	void									spawnEnemy(sf::Vector2f _pos);
	void									spawnEnemyRandom();

	/*********** Obstacles */
	void									spawnObstaclesFour();
	void									spawnObstaclesNarrow();
	void									spawnObstaclesRandom();

private:
	/*********** Units */
	std::shared_ptr<PlayerUnit>				unit;
	std::shared_ptr<sf::RenderWindow>		window;

	std::vector<std::shared_ptr<Obstacle>>	obstacles;
	std::vector<std::shared_ptr<Enemy>>		enemies;

	/*********** Settings */
	const float SPAWN_DELTA = 2.f;
	float spawnTimer = SPAWN_DELTA;
};

