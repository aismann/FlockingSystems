#pragma once
#include <Box2D/Box2D.h>

class PhysicNode
{
public:
	/*********** Contact event handlers */
	virtual void onBeginContact(b2Contact* contact) {};
	virtual void onEndContact(b2Contact* contact) {};
	virtual void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
	virtual void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};

};
