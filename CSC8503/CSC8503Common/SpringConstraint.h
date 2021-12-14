#pragma once
#include "Constraint.h"
#include "Spring.h"
#include "../../Common/Vector3.h"

namespace NCL
{
	namespace CSC8503
	{
		class GameObject;
		using namespace Maths;

		class SpringConstraint : public Constraint
		{
		public:

			SpringConstraint(GameObject* a, Vector3 origin, Spring* s)
			{
				object = a;
				this->origin = origin;
				this->spring = s;
			}
			~SpringConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 origin;
			Spring* spring;
		};
	}
}