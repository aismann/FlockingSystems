#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace fe {
	class Scene;

	class Node :
		public std::enable_shared_from_this<Node>
	{
	public:
		/*********** Constructor / Destructor */
		Node();
		virtual ~Node();

		/*********** Base */
		virtual void onBaseEvent(sf::Event& _event);
		virtual void onBaseUpdate(double _dt);
		virtual void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates());

		/*********** To implement in game */
		virtual void onInit();
		virtual void onExit();

		virtual void onEvent(sf::Event& _event);
		virtual void onUpdate(double _dt);
		virtual void onDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates());

		/*********** Transform */
		virtual sf::Transform getGlobalTransform(); // dummy transform to allow proper calls of onBaseDraw

		/*********** Tree structure */
		void addChild(std::shared_ptr<Node> _child);

		/*********** Settings */
		void setDeleted();
		bool isDeleted();

		void setDisabled(bool _val);
		bool isDisabled();

		/*********** Node id */
		int getNodeId();

	protected:
		/*********** Node id*/
		static int				NODE_ID;
		int						nodeId;

		/*********** Settings */
		bool					deleted;
		bool					disabled;

		/*********** Tree structure */
		std::weak_ptr<Node>					parent;
		std::vector<std::shared_ptr<Node>>	children;
		std::vector<std::shared_ptr<Node>>	reqChildren;
	};

}