#pragma once
#include "Node2D.h"

class ProgressBar :
	public fe::Node2D
{
public:
	/*********** Constructor / Destructor */
	ProgressBar(sf::Vector2f _pos, float _width, sf::Color _color = sf::Color(255, 0, 0));
	virtual ~ProgressBar();

	/*********** To implement in game */
	void onDraw(sf::RenderTarget& _target, sf::RenderStates _state) override;

	void setFillLevel(float _lvl);

protected:
	// Recompute fill vertices after change 
	void computeFill(float _fill);

protected:
	/*********** Components */
	sf::Vertex	v_fill[4];
	sf::Vertex	v_frame[4];

	/*********** Settings */
	static const float HEIGHT;
	
	sf::Vector2f pos;
	float width;
};

