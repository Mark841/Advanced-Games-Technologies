#include "GameObject.h"
#include "SpringConstraint.h"

using namespace NCL::CSC8503;

void SpringConstraint::UpdateConstraint(float dt)
{
	Vector3 relativePos = object->GetTransform().GetPosition() - origin;
	Vector3 pos = object->GetTransform().GetPosition();

	float offset = relativePos.Length() - spring->GetLength();
	if (abs(offset) > 0.01f)
	{
		Vector3 offsetDir = relativePos.Normalised();

		Vector3 force = (offsetDir * -(spring->GetSnappiness())) * spring->GetDamping();

		object->GetPhysicsObject()->ApplyLinearImpulse(force);
	}
}