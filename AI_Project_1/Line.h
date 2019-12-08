#pragma once
#include "Node2D.h"

class Line :
	public fe::Node2D
{
public:
	/*********** Constructor / Destructor */
	Line(sf::Vector2f _start, sf::Vector2f _end, float _thickness, sf::Color _color);
	Line(sf::Vector2f _start, sf::Vector2f _end);
	virtual ~Line();

	void setPoints(sf::Vector2f _start, sf::Vector2f _end);

	/*********** To implement in game */
	void onDraw(sf::RenderTarget& _target, sf::RenderStates _state) override;

protected:
	/*********** Components */
	sf::Vertex	vertices[4];
	float		thickness;
};

