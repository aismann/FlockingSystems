#include "Scene.h"
#include "SceneManager.h"
#include "Node2D.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::baseOnEvent(sf::Event& _event)
{
	for (auto& node : nodeList) {
		node->onEvent(_event);
	}

	this->onEvent(_event);
}

void Scene::baseOnUpdate(double _dt)
{
	for (auto& node : nodeList) {
		node->onUpdate(_dt);
	}

	this->onUpdate(_dt);
}

void Scene::baseOnDraw(sf::RenderTarget& _target)
{
	for (auto& node : nodeList) {
		node->onDraw(_target);
	}

	this->onDraw(_target);
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

void Scene::addNode(std::shared_ptr<Node2D> _node)
{
	auto it = std::find(nodeList.begin(), nodeList.end(), _node);
	if (it == nodeList.end()) {
		return;
	}

	nodeList.push_back(std::move(_node));
}

void Scene::deleteNode(std::shared_ptr<Node2D> _node)
{
	// Delete all _node from nodeList
	nodeList.erase(std::remove(nodeList.begin(), nodeList.end(), _node), nodeList.end());
}

bool Scene::isForceDraw()
{
	return forceDraw;
}

bool Scene::isForceUpdate()
{
	return forceUpdate;
}

void Scene::popFromStack()
{
	SceneManagerInstance.popScene(shared_from_this());
}

void Scene::pushToStack()
{
	SceneManagerInstance.pushScene(shared_from_this());
}
