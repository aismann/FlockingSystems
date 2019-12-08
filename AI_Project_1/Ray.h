#pragma once
#include "Line.h"

class Ray :
	public Line
{
public:
	/*********** Constructor / Destructor */
	Ray(sf::Vector2f _start, sf::Vector2f _end);
	virtual ~Ray();

	/*********** To implement in game */
	void onUpdate(double _dt) override;

private:
	/*********** Settings */
	float	startTimer = 1.0f;
	float	currTimer = startTimer;
};

