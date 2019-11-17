#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	/*********** Singleton */
	static SceneManager& getInstance();

	/*********** Constructor / Destructor */
	SceneManager();
	virtual ~SceneManager();

	/*********** Base */
	void onEvent(sf::Event& _event);
	void onUpdate(double _dt);
	void onDraw(sf::RenderTarget& _target);

	/*********** Pop / Push from stack */
	bool isEmpty();

	void popScene(Scene::Ptr _scene);
	void pushScene(Scene::Ptr _scene);

	void pushSceneNow(Scene::Ptr _scene);

private:
	/*********** List of nodes in this scene */
	std::vector<Scene::Ptr> sceneList;

	/*********** Push handle */
	std::vector<Scene::Ptr> sceneToPop;
	std::vector<Scene::Ptr> sceneToPush;
};

#define SceneManagerInstance SceneManager::getInstance() 
