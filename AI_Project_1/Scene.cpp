#include "Scene.h"
#include "SceneManager.h"
#include "Node2D.h"
#include "Engine.h"

namespace fe {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::onInit()
	{
	}

	void Scene::onExit()
	{
	}

	void Scene::baseOnEvent(sf::Event& _event)
	{
		onEvent(_event);

		for (auto& child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseEvent(_event);
		}
	}

	void Scene::baseOnUpdate(double _dt)
	{
		onUpdate(_dt);

		// Update all childs
		for (int i = 0; i < children.size(); /* conditional */) {
			auto child = children[i];

			if (child->isDisabled()) {
				continue;
			}

			child->onBaseUpdate(_dt);

			if (child->isDeleted()) {
				int lastIt = children.size() - 1;
				std::swap(children[i], children[lastIt]);

				child->onExit();
				children.pop_back();
			}
			else {
				i++;
			}
		}

		// Add requested childs
		for (auto ch : reqChildren) {
			ch->onInit();
			//ch->setParent(shared_from_this());
			children.push_back(ch);
		}
		reqChildren.clear();
	}

	void Scene::baseOnDraw(sf::RenderTarget& _target)
	{
		this->onDraw(_target);

		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseDraw(_target);
		}
	}

	void Scene::onEvent(sf::Event& _event)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Scene::onUpdate(double _dt)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Scene::onDraw(sf::RenderTarget& _target)
	{
		// TO BE IMPLEMENTED IN DERIVED SCENE
	}

	void Scene::addChild(std::shared_ptr<Node2D> _node)
	{
		auto it = std::find(children.begin(), children.end(), _node);
		if (it != children.end()) {
			return;
		}

		reqChildren.push_back(_node);
	}

	bool Scene::isForceDraw()
	{
		return forceDraw;
	}

	bool Scene::isForceUpdate()
	{
		return forceUpdate;
	}

	void Scene::popThisFromStack()
	{
		EngineInstance.getSceneManager()->reqPopScene(shared_from_this());
	}

	void Scene::pushThisToStack()
	{
		EngineInstance.getSceneManager()->reqPushScene(shared_from_this());
	}

}