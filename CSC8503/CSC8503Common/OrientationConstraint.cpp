#include "GameObject.h"
#include "OrientationConstraint.h"

using namespace NCL::CSC8503;

void OrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();
	Vector3 rotateDifference = (bRotation.ToEuler()) - aRotation.ToEuler();

	Quaternion relativeOrientation = objectA->GetTransform().GetOrientation() - objectB->GetTransform().GetOrientation();
	Vector3 offset = relativeOrientation.ToEuler();


	if (abs(offset.x) > 0.01f || abs(offset.y) > 0.01f || abs(offset.z) > 0.01f)
	{
		if (rotateDifference.x > angle)
		{
			offset.x = rotateDifference.x - angle;
		}
		if (rotateDifference.y > angle)
		{
			offset.y = rotateDifference.y - angle;
		}
		if (rotateDifference.z > angle)
		{
			offset.z = rotateDifference.z - angle;
		}
		
		//objectA->GetTransform().SetOrientation(aRotation + Quaternion(offset, aRotation.w));
		//objectB->GetTransform().SetOrientation(bRotation + Quaternion(-offset, bRotation.w));

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
		// - will filp the corners of rotation from front left to back right, but i need to swap from front left to front right
		//Vector3 bImpulse = Vector3(-offsetAngle.x, offsetAngle.y, -offsetAngle.z) * lambda * dt;
		Vector3 bImpulse = -offsetAngle * lambda * dt;

		physA->ApplyAngularImpulse(aImpulse);
		physB->ApplyAngularImpulse(bImpulse);

	}
	// Always tend towards being flat
	objectA->GetPhysicsObject()->ApplyAngularImpulse((Vector3(0, 0, 0) - aRotation.ToEuler()) * dt);
	objectB->GetPhysicsObject()->ApplyAngularImpulse((Vector3(0, 0, 0) - bRotation.ToEuler()) * dt);
}