#include "SceneManager.h"
#include <algorithm>

namespace fe {

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
			else if ((*it)->isForceUpdate()) {
				(*it)->baseOnUpdate(_dt);
			}
		}

		// Pop pending scenes
		auto it = sceneList.end();
		for (auto s : sceneToPop) {
			s->exit();

			it = std::remove(sceneList.begin(), sceneList.end(), s);
		}
		sceneList.erase(it, sceneList.end());
		sceneToPop.clear();

		// Push pending scene
		for (auto s : sceneToPush) {
			s->init();

			sceneList.push_back(s);
		}
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

	void SceneManager::reqPopScene(std::shared_ptr<Scene> _scene)
	{
		auto it = std::find(sceneToPop.begin(), sceneToPop.end(), _scene);
		if (it != sceneToPop.end()) {
			return;
		}

		sceneToPop.push_back(_scene);
	}

	void SceneManager::reqPushScene(std::shared_ptr<Scene> _scene)
	{
		auto it = std::find(sceneToPush.begin(), sceneToPush.end(), _scene);
		if (it != sceneToPush.end()) {
			return;
		}

		sceneToPush.push_back(_scene);
	}

	void SceneManager::pushSceneNow(std::shared_ptr<Scene> _scene)
	{
		auto it = std::find(sceneToPush.begin(), sceneToPush.end(), _scene);
		if (it != sceneToPush.end()) {
			return;
		}

		_scene->init();
		sceneList.push_back(_scene);
	}

}