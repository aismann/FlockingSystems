#include "Collider.h"

namespace fe {
	Collider::Collider()
	{
	}

	Collider::~Collider()
	{
	}

	std::weak_ptr<Node2D> Collider::getParentNode()
	{
		return this->parentNode;
	}


}