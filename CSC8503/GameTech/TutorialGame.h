#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../GameTech/StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame(int level);
			~TutorialGame();
			
			virtual void UpdateGame(float dt);
			void SetPaused(bool p) { paused = p; }

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();
			void InitWorld1();
			void InitWorld2();
			void DrawTextDebugs();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();

			void XAxisBridgeConstraintTest(const Vector3& position);
			void ZAxisBridgeConstraintTest(const Vector3& position);
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Vector3& position);
			void AddWallsToFloor();
			GameObject* AddLeftWallToWorld();
			GameObject* AddRightWallToWorld();
			GameObject* AddFrontWallToWorld();
			GameObject* AddBackWallToWorld();
			void AddWallSeperators();
			GameObject* AddWallToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddStartToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddFinishToWorld(const Vector3& position, const Vector3& size);

			void RampObstacles();
			void TiltingConstraintObstacles();
			void SpinningObstacles(const Vector3& centrePosition);
			void AddFlickerObjects(const Vector3& centrePosition);


			GameObject* AddSphereToWorld(int layer, const Vector3& position, float radius, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1,1,1,1));
			GameObject* AddAABBCubeToWorld(int layer, const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1));
			GameObject* AddOBBCubeToWorld(int layer, const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1));
			
			GameObject* AddCapsuleToWorld(int layer, const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1));

			GameObject* AddPlayerToWorld(int layer, const Vector3& position);
			GameObject* AddEnemyToWorld(int layer, const Vector3& position);
			GameObject* AddBonusToWorld(int layer, const Vector3& position);

			GameObject* AddPlayerBallToWorld(int layer, const Vector3& position, const float radius);
			void AddBallFlickerHorizontal(int layer, const Vector3& position, bool onLeft);
			void AddBallFlickerVertical(int layer, const Vector3& position, bool above);
			void AddBallPusherXAxis(int layer, const Vector3& position);
			void AddBallPusherZAxis(int layer, const Vector3& position);
			
			StateGameObject* AddStateSphereObjectToWorld(int layer, const ObjectMovement movement, const Vector3& position, const float radius, float inverseMass);
			StateGameObject* AddStateCubeObjectToWorld(int layer, const ObjectMovement movement, const Vector3& position, const Vector3 size, float inverseMass);
			StateGameObject* AddStateBonusObjectToWorld(int layer, const ObjectMovement movement, const Vector3& position, float inverseMass);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool finished;
			bool displayInfo;
			bool playerCanMoveBall;
			bool paused;
			bool inSelectionMode;
			int level;
			float totalTime;
			Vector4 moveableObjectColour = Vector4(0.5f, 1, 0.5f, 1);
			Vector4 pickupObjectColour = Vector4(0.5f, 1, 1, 1);

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* finish = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

		};
	}
}

