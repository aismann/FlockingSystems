#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace fe {

	class Node2D;

	class Scene:
		public std::enable_shared_from_this<Scene>
	{
	public:
		/*********** Constructor / Destructor */
		Scene();
		virtual ~Scene();

		/*********** Base */
		virtual void onInit();
		virtual void onExit();

		virtual void baseOnEvent(sf::Event& _event) final;
		virtual void baseOnUpdate(double _dt) final;
		virtual void baseOnDraw(sf::RenderTarget& _target) final;

		/*********** To implement in game */
		virtual void onEvent(sf::Event& _event);
		virtual void onUpdate(double _dt);
		virtual void onDraw(sf::RenderTarget& _target);

		/*********** Node tree */
		void addChild(std::shared_ptr<Node2D> _node);

		/*********** Settings */
		bool isForceDraw();
		bool isForceUpdate();

	public:
		/*********** Pop / Push from stack */
		void popThisFromStack();
		void pushThisToStack();

	private:
		/*********** Settings */
		bool forceDraw = false; // Draw when not on the top of state stack
		bool forceUpdate = false; // Update when not on the top of state stack

		/*********** Node tree */
		std::vector<std::shared_ptr<Node2D>> children;
		std::vector<std::shared_ptr<Node2D>> reqChildren; // Children requested to push
	};

}