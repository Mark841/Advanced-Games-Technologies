#pragma once
#include "../CSC8503Common/GameObject.h"


namespace NCL
{
	namespace CSC8503
	{

		class DestinationObject : public GameObject
		{
		public:
			DestinationObject(int layer = 0, string name = "");
			~DestinationObject() {}

			virtual void Update(float dt) {}

			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

			bool GetTriggered() const
			{
				return triggered;
			}

		protected:
			bool triggered;
		};
	}
}