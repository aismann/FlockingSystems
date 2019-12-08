#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Node.h"

namespace fe {

	class Scene;

	class Node2D :
		public Node,
		public sf::Transformable
	{
	public:
		/*********** Constructor / Destructor */
		Node2D(sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
		virtual ~Node2D();

		/*********** Base */
		void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates()) override;

		/*********** Transform */
		sf::Transform getGlobalTransform();
	};

}