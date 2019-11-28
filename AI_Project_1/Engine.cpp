#include "Engine.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

Engine& Engine::getInstance()
{
	static Engine instance;
	return instance;
}

void Engine::start(int argc, char** argv)
{
	/********** Box2D Initialization **********/
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	b2Vec2 gravity(0.0f, -10.0f);
	physicWorld = new b2World(gravity);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	contactListener = new ContactListener();
	physicWorld->SetContactListener(this->contactListener);
	/********** Box2D Initialization_END ******/

	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Project 1");
	const double DELTA = 1.0 / 120.0;
	
	sceneManager.pushSceneNow(new MainScene());

	// Global clock
	sf::Clock clock;
	double time = 0.0;

	this->isStarted = true;
	while (window.isOpen() && !sceneManager.isEmpty() && isStarted)
	{
		// Input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			sceneManager.onEvent(event);
			inputManager.takeEvent(event);
		}

		// Update
		if (time + clock.getElapsedTime().asSeconds() > DELTA) {
			time += clock.getElapsedTime().asSeconds();
			clock.restart();

			while (time > DELTA) {
				sceneManager.onUpdate(DELTA);

				physicWorld->Step((float)DELTA, velocityIterations, positionIterations);

				time -= DELTA;
			}
		}

		// Draw
		window.clear();
		sceneManager.onDraw(window);
		window.display();
	}
}

void Engine::shutdown()
{
	this->isStarted = false;
}

SceneManager& Engine::getSceneManager()
{
	return this->sceneManager;
}

InputManager& Engine::getInputManager()
{
	return this->inputManager;
}

b2World* Engine::getWorld()
{
	return physicWorld;
}

Engine::Engine():
	isStarted(false),
	contactListener(nullptr),
	physicWorld(nullptr)
{
}

Engine::~Engine()
{
	delete this->contactListener;
}
