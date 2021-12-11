#pragma once
#include "../CSC8503Common/GameObject.h"


namespace NCL
{
	namespace CSC8503
	{
		class StateMachine;

		enum class ObjectMovement
		{
			MOVING,
			ROTATING
		};

		class StateGameObject : public GameObject
		{
		public:
			StateGameObject(ObjectMovement movement, int layer = 0, string name = "");
			~StateGameObject();

			virtual void Update(float dt);

		protected:
			void InitMoving();
			void InitRotating();
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void RotateAnticlockwise(float dt);
			void RotateClockwise(float dt);

			StateMachine* stateMachine;
			float counter;
		};
	}
}