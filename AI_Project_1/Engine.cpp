#include "Engine.h"

#include <SFML/Graphics.hpp>

#include "SceneManager.h"
#include "InputManager.h"
#include "PhysicsEngine.h"

#include "Console.h"

#include "MainScene.h"

namespace fe {

	Engine& Engine::getInstance()
	{
		static Engine instance;
		return instance;
	}

	void Engine::start(int argc, char** argv)
	{
		this->mainWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "AI Project 1");
		const double DELTA = 1.0 / 120.0;

		sceneManager->pushSceneNow(std::make_shared<MainScene>());

		// Global clock
		sf::Clock clock;
		double time = 0.0;

		this->isStarted = true;
		while (mainWindow->isOpen() && !sceneManager->isEmpty() && isStarted)
		{
			// Input
			sf::Event event;
			while (mainWindow->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					mainWindow->close();

				sceneManager->onEvent(event);
				inputManager->takeEvent(event);
			}

			// Update
			if (time + clock.getElapsedTime().asSeconds() > DELTA) {
				time += clock.getElapsedTime().asSeconds();
				clock.restart();

				while (time > DELTA) {
					sceneManager->onUpdate(DELTA);
					time -= DELTA;
				}
			}

			// Draw
			mainWindow->clear();
			sceneManager->onDraw(*mainWindow);
			mainWindow->display();
		}
	}

	void Engine::shutdown()
	{
		this->isStarted = false;
	}

	std::shared_ptr<SceneManager> Engine::getSceneManager()
	{
		return this->sceneManager;
	}

	std::shared_ptr<InputManager> Engine::getInputManager()
	{
		return this->inputManager;
	}

	std::shared_ptr<sf::RenderWindow> Engine::getMainWindow()
	{
		return this->mainWindow;
	}

	Engine::Engine() :
		isStarted(false)
	{
		this->sceneManager = std::make_shared<SceneManager>();
		this->inputManager = std::make_shared<InputManager>();
		this->physicsEngine = std::make_shared<PhysicsEngine>();
	}

	Engine::~Engine()
	{
	}

}