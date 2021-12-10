#pragma once
#include "Constraint.h"

namespace NCL
{
	namespace CSC8503
	{
		class GameObject;

		class OrientationConstraint : public Constraint
		{
		public:

			OrientationConstraint(GameObject* a, GameObject* b, float angle)
			{
				objectA = a;
				objectB = b;
				this->angle = angle;
			}
			~OrientationConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;

			float angle;
		};
	}
}