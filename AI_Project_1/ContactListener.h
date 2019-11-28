#pragma once
#include <Box2D/Box2D.h>
#include "Console.h"

class ContactListener : public b2ContactListener 
{ 
public:
	void BeginContact(b2Contact* contact) {
		C_INFO("BeginContanct");
	}

	void EndContact(b2Contact* contact) {
		C_INFO("EndContanct");
	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
		C_INFO("PreSolve");
	}

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
		C_INFO("PostSolve");
	} 
};