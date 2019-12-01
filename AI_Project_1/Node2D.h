#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace fe {

	class Scene;

	class Node2D :
		public sf::Transformable,
		public std::enable_shared_from_this<Node2D>
	{
	public:
		/*********** Constructor / Destructor */
		Node2D(sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
		virtual ~Node2D();

		/*********** Base */
		virtual void onBaseEvent(sf::Event& _event);
		virtual void onBaseUpdate(double _dt);
		virtual void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates());

		/*********** To implement in game */
		virtual void onInit();
		virtual void onExit();

		virtual void onEvent(sf::Event& _event);
		virtual void onUpdate(double _dt);
		virtual void onDraw(sf::RenderTarget& _target);

		/*********** Transform */
		sf::Transform getGlobalTransform();

		/*********** Tree structure */
		void addChild(std::shared_ptr<Node2D> _child);

		/*********** Settings */
		void setDeleted();
		bool isDeleted();

		void setDisabled(bool _val);
		bool isDisabled();

	private:
		/*********** Settings */
		bool					deleted;
		bool					disabled;

		/*********** Tree structure */
		std::weak_ptr<Node2D>					parent;
		std::vector<std::shared_ptr<Node2D>>	children;
		std::vector<std::shared_ptr<Node2D>>	reqChildren;
	};

}