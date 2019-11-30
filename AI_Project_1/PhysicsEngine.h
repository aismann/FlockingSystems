#pragma once
#include <memory>
#include <vector>

namespace fe {
	class PhysicNode;

	class PhysicsEngine
	{
	public:
		/*********** Constructor / Destructor */
				PhysicsEngine();
		virtual ~PhysicsEngine();

		/*********** Simulation bodies */
		void addBody(std::shared_ptr<PhysicNode> _node);
		void removeBody(std::shared_ptr<PhysicNode> _node);

		std::vector<std::shared_ptr<PhysicNode>> raycast(sf::Vector2f _point, sf::Vector2f _dir);

		/*********** Update physics */
		void onUpdate(double _dt);

	private:
		/*********** Update physics */
		void checkCollision(std::shared_ptr<PhysicNode> _node1, std::shared_ptr<PhysicNode> _node2);

	private:
		/*********** Simulation bodies */
		std::vector<std::shared_ptr<PhysicNode>> bodies;

		// Delete tagged bodies 
		const float deleteInterval = 5.f;
		float deleteTimer = deleteDelta;
	};
};


