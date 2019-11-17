#include "MainScene.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::onEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::A) {
		printf("DUPA");
	}
}

void MainScene::onUpdate(double _dt)
{

}

void MainScene::onDraw(sf::RenderTarget& _target)
{

}
