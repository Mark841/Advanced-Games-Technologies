#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <vector>

using std::vector;

enum class States
{
	NO_STATE,
	MOVING_LEFT,
	MOVING_RIGHT,
	ROTATING_CLOCKWISE,
	ROTATING_ANTICLOCKWISE
};

namespace NCL {
	namespace CSC8503 {

		class GameObject	{
		public:
			GameObject(int layer = 0, string name = "", States state = States::NO_STATE, Vector4 baseColour = Vector4(1,1,1,1));
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			virtual void Update(float dt) {}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			Vector4 GetBaseColour() const {
				return baseColour;
			}

			const string& GetName() const {
				return name;
			}
			void SetState(States state) { this->state = state; }
			const string GetState() const {
				switch (state)
				{
				case (States::NO_STATE): return "NO STATE";
				case (States::MOVING_LEFT): return "MOVING LEFT";
				case (States::MOVING_RIGHT): return "MOVING RIGHT";
				case (States::ROTATING_CLOCKWISE): return "ROTATING CLOCKWISE";
				case (States::ROTATING_ANTICLOCKWISE): return "ROTATING ANTICLOCKWISE";
				}
				return "UNKNOWN STATE";
			}

			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}

			int		GetWorldID() const {
				return worldID;
			}

			void SetLayer(int layer)
			{
				this->layer = layer;
			}
			int GetLayer() const
			{
				return layer;
			}

		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;

			bool	isActive;
			int		worldID;
			int layer;
			string	name;
			States state;
			Vector4 baseColour;

			Vector3 broadphaseAABB;
		};
	}
}

