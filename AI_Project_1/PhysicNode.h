#pragma once
#include "Node2D.h"
#include "Math.h"

namespace fe {
	class Collider;
	struct Contact;

	class PhysicNode :
		public Node2D
	{
	public:
		enum Layer {
			RAY_CAST	= 1 << 0,
			COLLISION	= 1 << 1,
		};

	public:
		/*********** Constructor / Destructor */
		PhysicNode(sf::Vector2f _position = sf::Vector2f(0.f, 0.f));
		virtual ~PhysicNode();

		/*********** Contact event handlers */
		virtual void onBeginContact(Contact& _contact) {};

		/*********** Collider */
		void						setCollider(std::shared_ptr<Collider> _collider);
		std::shared_ptr<Collider>	getCollider();

		/*********** Settings */
		void setDeleted() override;

		void setLayerFlags(int _flags);
		bool isFlag(int _flags);

	public:
		/*********** Tools */
		sf::Vector2f pointToWorldSpace(sf::Vector2f _local);
		sf::Vector2f pointToLocalSpace(sf::Vector2f _local);

		sf::Vector2f vectorToWorldSpace(sf::Vector2f _local);
		sf::Vector2f vectorToLocalSpace(sf::Vector2f _world);

		/*********** Properties */
		float	getMass() { return this->mass; };
		void	setMass(float _mass) { this->mass = _mass; };

		float	getMaxSpeed() { return this->maxSpeed; };
		void	setMaxSpeed(float _speed) { this->maxSpeed = _speed; };

		float	getMaxForce() { return this->maxForce; };
		void	setMaxForce(float _force) { this->maxForce = _force; };

		sf::Vector2f	getVelocity() { return this->velocity; };
		void			setVelocity(sf::Vector2f _vect) { this->velocity = _vect; };

		sf::Vector2f	getHeading() { return this->heading; };
		void			setHeading(sf::Vector2f _vect);

		sf::Vector2f	getSide() { return this->side; };

		float			getSpeed() { return math::length(this->velocity);  };
		float			getSpeedSq() { return math::lengthSquare(this->velocity); };

	protected:
		/*********** Collider */
		std::shared_ptr<Collider>	collider;

		/*********** Properties */
		sf::Vector2f velocity;
		sf::Vector2f heading;
		sf::Vector2f side;

		float maxSpeed;	// m / s
		float maxForce;	// kg * m / s*s
		float mass;		// kg

		int layerFlag = ~(0);
	};
}
