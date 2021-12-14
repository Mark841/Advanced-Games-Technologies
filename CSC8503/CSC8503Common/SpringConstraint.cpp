#include "GameObject.h"
#include "SpringConstraint.h"

using namespace NCL::CSC8503;

void SpringConstraint::UpdateConstraint(float dt)
{
	Vector3 relativePos = object->GetTransform().GetPosition() - origin;
	Vector3 pos = object->GetTransform().GetPosition();

	float length = relativePos.Length() - spring->GetLength();

	if (abs(length) > 0.01f)
	{
		Vector3 offsetDir = relativePos.Normalised();
		float offset = relativePos.Length();

		Vector3 force = offsetDir * -(spring->GetSnappiness() * (length - spring->GetLength()));

		object->GetPhysicsObject()->ApplyLinearImpulse(force * dt);
	}
}