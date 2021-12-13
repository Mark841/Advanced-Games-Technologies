#include "DestinationObject.h"

using namespace NCL;
using namespace CSC8503;

DestinationObject::DestinationObject(int layer, string name) : GameObject(layer, name) 
{
	triggered = false;
}

void DestinationObject::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		triggered = true;
	}
}
void DestinationObject::OnCollisionEnd(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		triggered = false;
	}
}