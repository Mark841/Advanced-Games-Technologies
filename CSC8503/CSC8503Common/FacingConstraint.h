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
				objectA = a;
				objectB = nullptr;
				this->origin = origin;
			}
			FacingConstraint(GameObject* a, GameObject* b)
			{
				objectA = a;
				objectB = b;
				origin = Vector3();
			}
			~FacingConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;
			Vector3 origin;
		};
	}
}