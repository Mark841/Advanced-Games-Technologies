#include "GameObject.h"
#include "OrientationConstraint.h"

using namespace NCL::CSC8503;

void OrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();

	Vector3 offset = (bRotation.ToEuler()) - aRotation.ToEuler();
	Quaternion relativeOrientation = aRotation - bRotation;


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
	// Always tend towards being flat
	objectA->GetPhysicsObject()->ApplyAngularImpulse((Vector3(0, 0, 0) - aRotation.ToEuler()) * dt);
	objectB->GetPhysicsObject()->ApplyAngularImpulse((Vector3(0, 0, 0) - bRotation.ToEuler()) * dt);
}