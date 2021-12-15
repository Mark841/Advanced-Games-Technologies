#include "DestinationObject.h"

using namespace NCL;
using namespace CSC8503;

DestinationObject::DestinationObject(int layer, string name) : GameObject(layer, name) 
{
	triggeredBy = nullptr;
}

void DestinationObject::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL" || otherObject->GetName() == "STATE SPHERE")
	{
		triggeredBy = otherObject;
	}
}
void DestinationObject::OnCollisionEnd(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL" || otherObject->GetName() == "STATE SPHERE")
	{
		triggeredBy = nullptr;
	}
}