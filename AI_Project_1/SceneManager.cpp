#include "SceneManager.h"
#include <algorithm>

SceneManager& SceneManager::getInstance()
{
	static SceneManager instance;
	return instance;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::onEvent(sf::Event& _event)
{
	for (auto& scene : sceneList) {
		scene->baseOnEvent(_event);
	}
}

void SceneManager::onUpdate(double _dt)
{
	for (auto it = sceneList.rbegin(); it != sceneList.rend(); it++) {
		if (it == sceneList.rbegin()) {
			(*it)->baseOnUpdate(_dt);
		}
		else if((*it)->isForceUpdate()) {
			(*it)->baseOnUpdate(_dt);
		}
	}

	// Pop pending scenes
	auto it = sceneList.end();
	for (auto& s : sceneToPop) {
		it = std::remove(sceneList.begin(), sceneList.end(), s);
	}
	sceneList.erase(it, sceneList.end());

	// Push pending scene
	sceneList.insert(sceneList.end(), sceneToPush.begin(), sceneToPush.end());
	sceneToPush.clear();
}

void SceneManager::onDraw(sf::RenderTarget& _target)
{
	for (auto it = sceneList.rbegin(); it != sceneList.rend(); it++) {
		if (it == sceneList.rbegin()) {
			(*it)->baseOnDraw(_target);
		}
		else if ((*it)->isForceDraw()) {
			(*it)->baseOnDraw(_target);
		}
	}
}

bool SceneManager::isEmpty()
{
	return sceneList.empty();
}

void SceneManager::popScene(Scene::Ptr _scene)
{
	auto it = std::find(sceneToPop.begin(), sceneToPop.end(), _scene);
	if ( it == sceneToPop.end() ) {
		return;
	}

	sceneToPop.push_back(std::move(_scene));
}

void SceneManager::pushScene(Scene::Ptr _scene)
{
	auto it = std::find(sceneToPush.begin(), sceneToPush.end(), _scene);
	if (it != sceneToPush.end()) {
		return;
	}

	sceneToPush.push_back(std::move(_scene));
}

void SceneManager::pushSceneNow(Scene::Ptr _scene)
{
	auto it = std::find(sceneToPush.begin(), sceneToPush.end(), _scene);
	if (it != sceneToPush.end()) {
		return;
	}
	
	sceneList.push_back(std::move(_scene));
}
