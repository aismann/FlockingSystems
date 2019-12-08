#pragma once
#include "Node2D.h"

#include <memory>

namespace fe {
	class Node2D;

	class Collider:
		public sf::Transformable
	{
	public:
		/*********** Constructor / Destructor */
		Collider();
		virtual ~Collider();

		/*********** Transform */
		sf::Transform getGlobalTransform();

		/*********** Game object */
		void setGameObject(std::shared_ptr<Node> _gameObject);

	private:
		/*********** Game object */
		std::weak_ptr<Node> gameObject;
	};

}
