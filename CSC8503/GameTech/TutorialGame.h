#pragma once
#include <algorithm>
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../GameTech/StateGameObject.h"
#include "DestinationObject.h"
#include "PowerUpObject.h"

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
			void InitWorld3();
			void DrawTextDebugs();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitCapsuleGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitDefaultFloor();

			void InitLevelOneMap(const Vector3& centre);
			void InitLevelTwoMap(const Vector3& centre);

			void XAxisBridgeConstraint(const Vector3& position, int length);
			void ZAxisBridgeConstraint(const Vector3& position, int length);
			GameObject* AttachableRopeConstraint(const Vector3& topPosition, int length);
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddAngledFloorToWorld(const Vector3& position, const Vector3& size, const Vector3& angle);
			GameObject* AddIceToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddSlimeToWorld(const Vector3& position, const Vector3& size);

			void AddWallsToFloor(const Vector3& centre);
			GameObject* AddLeftWallToWorld(const Vector3& centre);
			GameObject* AddRightWallToWorld(const Vector3& centre);
			GameObject* AddFrontWallToWorld(const Vector3& centre);
			GameObject* AddBackWallToWorld(const Vector3& centre);

			void AddMazeFloor(const Vector3& centre);
			void AddMazeWalls(const Vector3& centre);

			void AddFunnel(const Vector3& holeCentre, float heightAboveFloor);
			void AddFunnelFloor(const Vector3& holeCentre, float heightAboveFloor);
			void AddFunnelFloorWithObstacles(const Vector3& holeCentre, float heightAboveFloor);

			void AddWallSeperators(const Vector3& centre);
			GameObject* AddWallToWorld(const Vector3& position, const Vector3& size);
			DestinationObject* AddStartToWorld(const Vector3& position, const Vector3& size);
			DestinationObject* AddFinishToWorld(const Vector3& position, const Vector3& size);
			DestinationObject* AddCheckpointToWorld(const Vector3& position, const Vector3& size);
			DestinationObject* AddKillPlaneToWorld(const Vector3& position, const Vector3& size);

			void TiltingConstraintObstacles(const Vector3& centre);
			void SpinningObstacles(const Vector3& centrePosition);
			void MovingObstacles(const Vector3& centrePosition);
			void AddFlickerObjectsZAxis(const Vector3& centrePosition);

			GameObject* AddSphereToWorld(int layer, const Vector3& position, float radius, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1,1,1,1));
			GameObject* AddAABBCubeToWorld(int layer, const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1), string name = "AABB CUBE");
			GameObject* AddOBBCubeToWorld(int layer, const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1), string name = "OBB CUBE");
			
			GameObject* AddCapsuleToWorld(int layer, const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f, bool moveable = false, Vector4 baseColour = Vector4(1, 1, 1, 1));

			GameObject* AddPlayerToWorld(int layer, const Vector3& position);
			GameObject* AddEnemyToWorld(int layer, const Vector3& position);
			GameObject* AddBonusToWorld(int layer, const Vector3& position);

			GameObject* AddPlayerBallToWorld(int layer, const Vector3& position, const float radius);
			void AddBallFlickerHorizontalZ(int layer, const Vector3& position, bool onLeft);
			void AddBallFlickerVertical(int layer, const Vector3& position, bool above);
			void AddBallPusherXAxis(int layer, const Vector3& position);
			void AddBallPusherZAxis(int layer, const Vector3& position);
			void AddSpringPusherXAxis(int layer, const Vector3& position, const Vector3& size, float length, float snappiness, float damping);
			void AddSpringPusherZAxis(int layer, const Vector3& position, const Vector3& size, float length, float snappiness, float damping);
			
			StateGameObject* AddStateSphereObjectToWorld(int layer, const ObjectMovement movement, const Vector3& position, const float radius, float inverseMass, Vector4 colour = Vector4(1, 1, 1, 1));
			StateGameObject* AddStateCubeObjectToWorld(int layer, const ObjectMovement movement, const Vector3& position, const Vector3& size, float inverseMass, Vector4 colour = Vector4(1, 1, 1, 1));
			PowerUpObject* AddPowerUpObjectToWorld(int layer, string name, const Vector3& position, float inverseMass, PowerUp ability);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool finished;
			bool displayInfo;
			bool playerCanMoveBall;
			bool paused;
			bool inSelectionMode;
			bool speedPowerUpActive;
			float speedPowerUpTimer;
			int level;
			bool reset;
			float totalTime;
			Vector3 respawnPoint;
			Vector3 mapCentre;
			Vector4 moveableObjectColour = Vector4(0.5f, 1, 0.5f, 1);
			Vector4 pickupObjectColour = Vector4(0.5f, 1, 1, 1);
			Vector4 checkpointColour = Vector4(1, 0.5f, 0.5f, 1);
			Vector4 enemyColour = Vector4(1, 0, 0, 1);

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* endPoint = nullptr;
			Constraint* attachedBallConstraint = nullptr;
			DestinationObject* finish = nullptr;
			DestinationObject* killPlane = nullptr;
			std::vector<PowerUpObject*> powerUps;
			std::vector<DestinationObject*> checkpoints;
			std::vector<StateGameObject*> enemies;
			GameObject* playerBall = nullptr;

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
			Vector3 lockedOffset		= Vector3(0, 75, 100);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

		};
	}
}

