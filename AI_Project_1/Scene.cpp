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

	void Scene::init()
	{
	}

	void Scene::exit()
	{
	}

	void Scene::baseOnEvent(sf::Event& _event)
	{
		this->onEvent(_event);

		for (auto& child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseEvent(_event);
		}
	}

	void Scene::baseOnUpdate(double _dt)
	{
		this->onUpdate(_dt);

		// Delete flagged childs
		auto pastEndIt = std::remove_if(children.begin(), children.end(), [](auto node) {return node->isDeleted(); });
		children.erase(pastEndIt, children.end());

		// Update all childs
		for (auto child : children) {
			if (child->isDisabled()) {
				continue;
			}

			child->onBaseUpdate(_dt);
		}

		// Add requested childs
		for (auto ch : reqChildren) {
			//s->init();
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