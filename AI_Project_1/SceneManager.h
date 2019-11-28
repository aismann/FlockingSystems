#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	/*********** Constructor / Destructor */
	SceneManager();
	virtual ~SceneManager();

	/*********** Base */
	void onEvent(sf::Event& _event);
	void onUpdate(double _dt);
	void onDraw(sf::RenderTarget& _target);

	/*********** Pop / Push from stack */
	bool isEmpty();

	void popScene(Scene* _scene);
	void pushScene(Scene* _scene);

	void pushSceneNow(Scene* _scene);

private:
	/*********** List of nodes in this scene */
	std::vector<Scene*> sceneList;

	/*********** Push handle */
	std::vector<Scene*> sceneToPop;
	std::vector<Scene*> sceneToPush;
};