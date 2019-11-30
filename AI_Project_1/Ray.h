#pragma once
#include "Node2D.h"

class Ray :
	public fe::Node2D
{
public:
	/*********** Constructor / Destructor */
	Ray(sf::Vector2f _start, sf::Vector2f _end);
	virtual ~Ray();

	/*********** To implement in game */
	void onUpdate(double _dt) override;
	void onDraw(sf::RenderTarget& _target) override;

private:
	/*********** Components */
	sf::Vertex vertices[4];

	void updateColor();

	/*********** Settings */
	float	startTimer = 1.0f;
	float	currTimer = startTimer;
};

