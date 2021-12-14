#include "GameObject.h"
#include "LockingPositionConstraint.h"

using namespace NCL::CSC8503;

void LockingPositionConstraint::UpdateConstraint(float dt)
{
	objectA->GetTransform().SetPosition(position);
}