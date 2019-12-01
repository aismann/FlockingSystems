#pragma once
#include "Scene.h"
#include "PlayerUnit.h"

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

private:
	/*********** Units */
	std::shared_ptr<PlayerUnit> unit;
};

