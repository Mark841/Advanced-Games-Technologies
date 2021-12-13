#include "PowerUpObject.h"

using namespace NCL;
using namespace CSC8503;

PowerUpObject::PowerUpObject(int layer, string name, PowerUp powerUp) : DestinationObject(layer, name)
{
	ability = powerUp;
}

void PowerUpObject::Update(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,-1,0 });
}