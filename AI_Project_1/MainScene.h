#pragma once
#include "Scene.h"
#include "PlayerUnit.h"

class MainScene : public Scene
{
public:
	/*********** Constructor / Destructor */
	MainScene();
	virtual ~MainScene();

	/*********** Base */
	void init() override;
	void exit() override;

	void onEvent(sf::Event& _event) override;
	void onUpdate(double _dt) override;
	void onDraw(sf::RenderTarget& _target) override;

private:
	/*********** Units */
	PlayerUnit* unit;
};

