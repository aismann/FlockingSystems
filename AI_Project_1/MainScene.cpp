#include "MainScene.h"
#include "Engine.h"

#include "Shape2D.h"

MainScene::MainScene():
	unit(nullptr)
{
}

MainScene::~MainScene()
{
}

void MainScene::init()
{
	unit = new PlayerUnit(this);
}

void MainScene::exit()
{
}

void MainScene::onEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::A) {
		//
	}
}

void MainScene::onUpdate(double _dt)
{
	if (InputManagerInstance.isKeyPressed(sf::Keyboard::S)) {
		//
	}
}

void MainScene::onDraw(sf::RenderTarget& _target)
{

}
