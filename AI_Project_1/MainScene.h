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
	std::vector<std::weak_ptr<Obstacle>>&	getObstacles();
	std::vector<std::weak_ptr<Enemy>>&		getEnemies();
	std::weak_ptr<PlayerUnit>				getPlayerUnit();

	void									tagEnemiesInRange(sf::Vector2f _start, float _range);

private:
	/*********** Units */
	void									spawnEnemy(sf::Vector2f _pos);

private:
	/*********** Units */
	std::shared_ptr<PlayerUnit>				unit;

	std::vector<std::weak_ptr<Obstacle>>	obstacles;
	std::vector<std::weak_ptr<Enemy>>		enemies;
};

