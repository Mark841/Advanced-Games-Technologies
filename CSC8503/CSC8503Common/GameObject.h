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
	ROTATING_ANTICLOCKWISE,
	SPINNING_CLOCKWISE,
	SPINNING_ANTICLOCKWISE,
	INACTIVE,
	REACHED
};

namespace NCL {
	namespace CSC8503 {

		class GameObject	{
		public:
			GameObject(int layer = 0, string name = "", States state = States::NO_STATE, Vector4 baseColour = Vector4(1,1,1,1), bool moveable = false, GameObject* attachSpring = nullptr);
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			void SetActive(bool active) {
				isActive = active;
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
			bool GetPlayerMoveable() const {
				return playerMoveable;
			}

			virtual void Update(float dt) {}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}
			bool HasSpringObject() {
				 return (attachedSpring == nullptr) ? false : true;
			}
			void AttachSpringObject(GameObject* otherObject, float springLength, float springSnappiness, float springDampingConstant) {
				attachedSpring = otherObject;
				this->springLength = springLength;
				this->springSnappiness = springSnappiness;
				this->springDampingConstant = springDampingConstant;
			}

			void SetBaseColour(Vector4 colour) {
				baseColour = colour;
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
				case (States::SPINNING_CLOCKWISE): return "SPINNING CLOCKWISE";
				case (States::SPINNING_ANTICLOCKWISE): return "SPINNING ANTICLOCKWISE";
				case (States::INACTIVE): return "INACTIVE";
				case (States::REACHED): return "REACHED";
				}
				return "UNKNOWN STATE";
			}

			virtual void OnCollisionBegin(GameObject* otherObject);

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
			bool playerMoveable;
			string	name;
			States state;
			Vector4 baseColour;

			// Variables to store spring information
			GameObject* attachedSpring;
			// Set spring resting length between 2 centres
			float springLength;
			float springSnappiness;
			float springDampingConstant;

			Vector3 broadphaseAABB;
		};
	}
}

