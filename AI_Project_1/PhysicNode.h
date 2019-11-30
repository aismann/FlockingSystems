#pragma once
#include "Node2D.h"

namespace fe {
	class Collider;
	struct Contact;

	class PhysicNode :
		public Node2D
	{
	public:
		/*********** Constructor / Destructor */
		PhysicNode(sf::Vector2f _origin = sf::Vector2f(0.f, 0.f));
		virtual ~PhysicNode();

		/*********** Contact event handlers */
		virtual void onBeginContact(std::shared_ptr<Contact> _contact) {};

		/*********** Collider */
		void setCollider(std::shared_ptr<Collider> _collider);
		std::shared_ptr<Collider> getCollider();

	private:
		/*********** Collider */
		std::shared_ptr<Collider>	collider;

	};
}
