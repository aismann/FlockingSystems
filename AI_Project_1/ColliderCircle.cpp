#include "ColliderCircle.h"

namespace fe {

	ColliderCircle::ColliderCircle(float _radius):
		radius(_radius),
		Collider()
	{
	}

	ColliderCircle::~ColliderCircle()
	{
	}

	float ColliderCircle::getRadius()
	{
		return radius;
	}

}