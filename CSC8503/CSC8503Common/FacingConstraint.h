#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL
{
	namespace CSC8503
	{
		class GameObject;
		using namespace Maths;

		class FacingConstraint : public Constraint
		{
		public:

			FacingConstraint(GameObject* a, Vector3 origin)
			{
				object = a;
				this->origin = origin;
			}
			~FacingConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 origin;

			float xAngle;
			float yAngle;
		};
	}
}