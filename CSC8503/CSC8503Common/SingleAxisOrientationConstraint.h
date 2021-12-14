#pragma once
#include "Constraint.h"


namespace NCL
{
	namespace CSC8503
	{
		class GameObject;
		
		class SingleAxisOrientationConstraint : public Constraint
		{
		public:

			SingleAxisOrientationConstraint(GameObject* a, GameObject* b, float angle, Axis axis)
			{
				objectA = a;
				objectB = b;
				this->angle = angle;
				this->axis = axis;
			}
			~SingleAxisOrientationConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;

			float angle;
			Axis axis;
		};
	}
}

