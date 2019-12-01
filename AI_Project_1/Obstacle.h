#pragma once
#include "PhysicNode.h"

namespace fe {
	class Shape2D;
}

class Obstacle
	: public fe::PhysicNode
{
public:
	/*********** Constructor / Destructor */
			Obstacle(sf::Vector2f _position);
	virtual ~Obstacle();

	/*********** Base */
	void onInit() override;

private:
	/*********** Components */

};
