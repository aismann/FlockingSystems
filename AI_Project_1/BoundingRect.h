#pragma once
#include "Node2D.h"

class BoundingRect :
	public fe::Node2D
{
public:
	/*********** Constructor / Destructor */
	BoundingRect(sf::Vector2f _start, sf::Vector2f _end, float _thickness, sf::Color _color);
	virtual ~BoundingRect();

	void setPoints(sf::Vector2f _start, sf::Vector2f _end);

	/*********** To implement in game */
	void onDraw(sf::RenderTarget& _target, sf::RenderStates _state) override;

protected:
	/*********** Components */
	sf::Vertex	vertices[5];
	float		thickness;
};

