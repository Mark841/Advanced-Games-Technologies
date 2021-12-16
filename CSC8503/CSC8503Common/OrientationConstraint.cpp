#include "GameObject.h"
#include "OrientationConstraint.h"

using namespace NCL::CSC8503;

void OrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();
	
	//Vector3 offset = Vector3(angle, angle, angle) - (bRotation.ToEuler() - aRotation.ToEuler());
	Vector3 offset = (bRotation.ToEuler() - aRotation.ToEuler());

	if (abs(offset.x) > 0.01f || abs(offset.y) > 0.01f || abs(offset.z) > 0.01f)
	{		
		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 offsetAngle = offset.Normalised();

		Vector3 relativeVelocity = physA->GetAngularVelocity() - physB->GetAngularVelocity();
		float velocityDot = Vector3::Dot(relativeVelocity, offsetAngle);

		float constraintMass = physA->GetInverseMass() + physB->GetInverseMass();

		float biasFactor = 0.01f;
		float bias = -(biasFactor / dt) * offset.Length();

		float lambda = -(velocityDot + bias) / constraintMass;

		Vector3 aImpulse = offsetAngle * lambda * dt;
		Vector3 bImpulse = -offsetAngle * lambda * dt;

		physA->ApplyAngularImpulse(aImpulse);
		physB->ApplyAngularImpulse(bImpulse);
	}
}