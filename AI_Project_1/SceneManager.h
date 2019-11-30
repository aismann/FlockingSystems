#pragma once
#include <vector>
#include "Scene.h"

namespace fe {

	class SceneManager
	{
	public:
		/*********** Constructor / Destructor */
		SceneManager();
		virtual ~SceneManager();

		/*********** Base */
		void onEvent(sf::Event& _event);
		void onUpdate(double _dt);
		void onDraw(sf::RenderTarget& _target);

		/*********** Pop / Push from stack */
		bool isEmpty();

		void reqPopScene(std::shared_ptr<Scene> _scene);
		void reqPushScene(std::shared_ptr<Scene> _scene);

		void pushSceneNow(std::shared_ptr<Scene> _scene);

	private:
		/*********** List of nodes in this scene */
		std::vector<std::shared_ptr<Scene>> sceneList;

		/*********** Push handle */
		std::vector<std::shared_ptr<Scene>> sceneToPop;
		std::vector<std::shared_ptr<Scene>> sceneToPush;
	};

}