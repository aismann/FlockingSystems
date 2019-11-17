#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Scene;

class Node2D: public std::enable_shared_from_this<Node2D>
{
public:
	using Ptr = std::shared_ptr<Node2D>;

	/*********** Constructor / Destructor */
	Node2D(std::shared_ptr<Scene> _scene = nullptr, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	Node2D(Node2D::Ptr _parent = nullptr, sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
	virtual ~Node2D();

	/*********** Base */
	virtual void onEvent(sf::Event& _event);
	virtual void onUpdate(double _dt);
	virtual void onDraw(sf::RenderTarget& _target);

	/*********** Position */
	void setOrigin(sf::Vector2f _pos);
	sf::Vector2f getOrigin();

	/*********** Rotation */
	void setRotation(float _rot);
	float getRotation();

	/*********** Tree structure */
	void setParent(Node2D::Ptr _parent);

	void addChild(Node2D::Ptr _child);
	void removeChild(Node2D::Ptr _child);

private:
	/*********** Transform */
	sf::Vector2f	origin;
	float			rotation;

	/*********** Tree structure */
	Node2D::Ptr					parent;
	std::vector<Node2D::Ptr>	children;
};

