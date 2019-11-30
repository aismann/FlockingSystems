#pragma once
#include "Node2D.h"

namespace fe {
	class Shape2D :
		public Node2D
	{
	public:
		/*********** Constructor / Destructor */
		Shape2D(sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
		virtual ~Shape2D();

		/*********** Base */
		virtual void onBaseDraw(sf::RenderTarget& _target, sf::RenderStates _state = sf::RenderStates()) override;

		/*********** Internal shape */
		void setShape(std::shared_ptr<sf::Shape> _shape);
		std::shared_ptr<sf::Shape> getShape();

	private:
		/*********** Internal shape */
		std::shared_ptr<sf::Shape> shape;
	};

}