#pragma once
#include "../CSC8503Common/GameObject.h"
#include "PowerUpObject.h"
#include "../CSC8503Common/NavigationGrid.h"

namespace NCL
{
	namespace CSC8503
	{
		class StateMachine;

		enum class ObjectMovement
		{
			MOVING,
			ROTATING,
			SPIN,
			AI
		};

		class StateGameObject : public GameObject
		{
		public:
			StateGameObject(ObjectMovement movement, int layer = 0, string name = "");
			~StateGameObject();

			virtual void Update(float dt);
			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

			int DistanceToObject(GameObject* otherObject);

			void AddToForceMultiplier(float multiplier) { forceMultiplier += multiplier; }
			void UpdatePowerUps(std::vector<PowerUpObject*> powerUps) { this->powerUps = powerUps; }
			void UpdatePlayerPos(GameObject* player) { this->playerBall = player; }
			bool EliminatedPlayer() const { return collisionWithPlayerBall; }

		protected:
			void InitMoving();
			void InitRotating();
			void InitSpinning();
			void InitAI();
			
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void RotateAnticlockwise(float dt);
			void RotateClockwise(float dt);
			void SpinAnticlockwise(float dt);
			void SpinClockwise(float dt);
			void Inactive();
			void Hunt(float dt, int huntingTarget);
			void Wander(float dt);
			bool Pathfind(const Vector3& startPos, const Vector3& endPos);
			void GenerateNewDestination();

			GameObject* collisionWithPlayerBall;
			GameObject* playerBall;
			std::vector<PowerUpObject*> powerUps;

			StateMachine* stateMachine;
			float counter;
			float forceMultiplier;
			Vector3 wanderDestination;
			vector<Vector3> nodes;
		};
	}
}