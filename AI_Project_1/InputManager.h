#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <bitset>

class InputManager
{
public:
	using KeyCode = sf::Keyboard::Key;

	/*********** Constructor / Destructor */
	InputManager();
	~InputManager();

	/*********** Check events */
	bool isKeyPressed(KeyCode _key);

	/*********** Handle events */
	void takeEvent(sf::Event& _event);

private:
	/*********** Events holder */
	std::bitset<KeyCode::KeyCount> keyPressedMap;
};

