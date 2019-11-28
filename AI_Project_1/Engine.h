#pragma once
#include "SceneManager.h"
#include "InputManager.h"

#include "Console.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "ContactListener.h"

class Engine
{
public:
	/*********** Singleton */
	static Engine& getInstance();

	/*********** Manage */
	void start(int argc, char** argv);
	void shutdown();

	/*********** Subsystems */
	SceneManager&	getSceneManager();
	InputManager&	getInputManager();

	b2World*		getWorld();

private:
	/*********** Constructor / Destructor */
	Engine();
	~Engine();

private:
	/*********** Manage */
	bool isStarted;

	/*********** Subsystems */
	SceneManager	sceneManager;
	InputManager	inputManager;

	b2World*			physicWorld;
	b2ContactListener*	contactListener;
};

#define EngineInstance			Engine::getInstance()
#define InputManagerInstance	Engine::getInstance().getInputManager()
#define SceneManagerInstance	Engine::getInstance().getSceneManager()
#define WorldInstance			Engine::getInstance().getWorld()

