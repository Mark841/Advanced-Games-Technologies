#pragma once
#include "DestinationObject.h"


namespace NCL
{
	namespace CSC8503
	{
		enum class PowerUp
		{
			DECREASE_FRICTION,
			INCREASE_FRICTION,
			ATTACH,
			DECREASE_TIME,
			SPEED_UP,
			NONE
		};
		class PowerUpObject : public DestinationObject
		{
		public:
			PowerUpObject(int layer = 0, string name = "", PowerUp powerUp = PowerUp::NONE);
			~PowerUpObject() {}

			virtual void Update(float dt);
			
			PowerUp GetAbility() const
			{
				return ability;
			}
		protected:
			PowerUp ability;
		};
	}
}
