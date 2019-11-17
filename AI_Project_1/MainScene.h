#pragma once
#include "Scene.h"

class MainScene : public Scene
{
public:
	/*********** Constructor / Destructor */
	MainScene();
	virtual ~MainScene();

	/*********** Base */
	virtual void onEvent(sf::Event& _event) override;
	virtual void onUpdate(double _dt) override;
	virtual void onDraw(sf::RenderTarget& _target) override;

private:
	/*********** Units */

};

