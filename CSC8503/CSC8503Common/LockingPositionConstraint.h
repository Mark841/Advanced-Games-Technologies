#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL
{
	namespace CSC8503
	{
		class GameObject;
		using namespace Maths;

		class LockingPositionConstraint : public Constraint
		{
		public:
			LockingPositionConstraint(GameObject* a, Vector3 position)
			{
				objectA = a;
				this->position = position;
			}
			~LockingPositionConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			Vector3 position;
		};
	}
}