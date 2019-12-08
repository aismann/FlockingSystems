#include "InputManager.h"

namespace fe {

	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
	}

	bool InputManager::isKeyPressed(KeyCode _key)
	{
		return this->keyPressedMap[_key];
	}

	void InputManager::takeEvent(sf::Event& _event)
	{
		if (_event.key.code < 0 || _event.key.code > (KeyCode::KeyCount - 1)) {
			return;
		}

		if (_event.type == sf::Event::KeyPressed) {
			this->keyPressedMap[_event.key.code] = true;
		}
		if (_event.type == sf::Event::KeyReleased) {
			this->keyPressedMap[_event.key.code] = false;
		}
	}

}