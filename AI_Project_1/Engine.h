#pragma once
#include <memory>

namespace sf {
	class RenderWindow;
}

namespace fe {
	class SceneManager;
	class InputManager;
	class PhysicsEngine;

	class Engine
	{
	public:
		/*********** Singleton */
		static Engine& getInstance();

		/*********** Manage */
		void start(int argc, char** argv);
		void shutdown();

		/*********** Subsystems */
		std::shared_ptr<SceneManager> getSceneManager();
		std::shared_ptr<InputManager> getInputManager();
		std::shared_ptr<PhysicsEngine> getPhysicEngine();
		
		std::shared_ptr<sf::RenderWindow> getMainWindow();

	private:
		/*********** Constructor / Destructor */
		Engine();
		~Engine();

	private:
		/*********** Manage */
		bool							isStarted;

		/*********** Subsystems */
		std::shared_ptr<sf::RenderWindow> mainWindow;

		std::shared_ptr<SceneManager>	sceneManager;
		std::shared_ptr<InputManager>	inputManager;
		std::shared_ptr<PhysicsEngine>	physicsEngine;
	};

	#define EngineInstance			Engine::getInstance()
}