#pragma once
#include <memory>

#include "Collider.h"

namespace fe {
	class Node2D;

	class ColliderCircle: 
		public Collider
	{
	public:
		/*********** Constructor / Destructor */
		ColliderCircle(float _radius);
		virtual ~ColliderCircle();

		/*********** Collider data */
		float getRadius();

	private:
		/*********** Collider data */
		float	radius;
	};

}
