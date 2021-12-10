#pragma once
#include "../CSC8503Common/GameObject.h"

namespace NCL
{
	namespace CSC8503
	{
		class StateMachine;
		class StateGameObject : public GameObject
		{
		public:
			StateGameObject(int layer = 0, string name = "");
			~StateGameObject();

			virtual void Update(float dt);

		protected:
			void InitMoving();
			void InitRotating();
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void RotateLeft(float dt);
			void RotateRight(float dt);

			StateMachine* stateMachine;
			float counter;
		};
	}
}