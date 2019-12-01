#pragma once
#include <SFML/System.hpp>

#include "PhysicNode.h"

class Enemy:
	public fe::PhysicNode
{
public:
	/*********** Constructor / Destructor */
			Enemy(sf::Vector2f _position = sf::Vector2f());
	virtual ~Enemy();

	/*********** Base */
	void onInit() override;

	/*********** Game logic */
	void rayHit();

private:
	/*********** AI */
	//std::shared_ptr<AIController> aiController;
};
