#pragma once
#include <memory>

#include <SFML/Graphics/Transformable.hpp>

namespace fe {
	class Node2D;

	class Collider:
		public sf::Transformable
	{
	public:
		/*********** Constructor / Destructor */
		Collider();
		virtual ~Collider();

		/*********** Parent node */
		std::weak_ptr<Node2D> getParentNode();

	private:
		/*********** Parent node */
		std::weak_ptr<Node2D> parentNode;
	};

}
