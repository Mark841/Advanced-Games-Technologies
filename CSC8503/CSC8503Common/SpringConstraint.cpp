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

		if (axis == Axis::PITCH)
		{
			force = Vector3(force.x, 0, 0);
		}
		if (axis == Axis::YAW)
		{
			force = Vector3(0, force.y, 0);
		}
		if (axis == Axis::ROLL)
		{
			force = Vector3(0, 0, force.z);
		}

		object->GetPhysicsObject()->SetLinearVelocity(object->GetPhysicsObject()->GetLinearVelocity() + (force * dt));
	}
}