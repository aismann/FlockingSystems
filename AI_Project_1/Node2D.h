#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Scene;

class Node2D: 
	public sf::Transformable
{
public:
	/*********** Constructor / Destructor */
	Node2D(Scene* _scene, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	Node2D(Node2D* _parent, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~Node2D();

	/*********** Base */
	virtual void onBaseEvent(sf::Event& _event);
	virtual void onBaseUpdate(double _dt);
	virtual void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates());

	/*********** To implement in game */
	virtual void onEvent(sf::Event& _event);
	virtual void onUpdate(double _dt);
	virtual void onDraw(sf::RenderTarget& _target);

	/*********** Tree structure */
	void setParent(Node2D* _parent);

	void addChild(Node2D* _child);
	void removeChild(Node2D* _child);

private:
	/*********** Tree structure */
	Node2D*					parent;
	std::vector<Node2D*>	children;
};

