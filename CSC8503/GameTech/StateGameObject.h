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
			ROTATING,
			SPIN
		};

		class StateGameObject : public GameObject
		{
		public:
			StateGameObject(ObjectMovement movement, int layer = 0, string name = "");
			~StateGameObject();

			virtual void Update(float dt);
			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

		protected:
			void InitMoving();
			void InitRotating();
			void InitSpinning();

			void MoveLeft(float dt);
			void MoveRight(float dt);
			void RotateAnticlockwise(float dt);
			void RotateClockwise(float dt);
			void SpinAnticlockwise(float dt);
			void SpinClockwise(float dt);
			void Inactive();

			GameObject* collisionWithPlayerBall;

			StateMachine* stateMachine;
			float counter;
		};
	}
}