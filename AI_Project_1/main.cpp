// AI_Project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Console.h"
#include "SceneManager.h"

#include "MainScene.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Project 1");
	const double DELTA = 1.0 / 120.0;

	auto scene = std::make_shared<MainScene>();
	SceneManagerInstance.pushSceneNow(scene);

	// Global clock
	sf::Clock clock;
	double time = 0.0;

	while (window.isOpen() && !SceneManagerInstance.isEmpty())
	{
		// Input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			SceneManagerInstance.onEvent(event);

		}

		// Update
		if (time + clock.getElapsedTime().asSeconds() > DELTA) {
			time += clock.getElapsedTime().asSeconds();
			clock.restart();

			while (time > DELTA) {
				SceneManagerInstance.onUpdate(DELTA);
				time -= DELTA;
			}
		}
		
		// Draw
		window.clear();
		SceneManagerInstance.onDraw(window);
		window.display();
	}

	return 0;
}