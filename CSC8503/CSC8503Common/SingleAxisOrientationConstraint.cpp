#include "GameObject.h"
#include "SingleAxisOrientationConstraint.h"

using namespace NCL::CSC8503;

void SingleAxisOrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();

	float angleTo;
	switch (axis)
	{
	case(Axis::PITCH): 
		angleTo = (aRotation.ToEuler().x > angle) ? angle : aRotation.ToEuler().x;
		angleTo = (aRotation.ToEuler().x < -angle) ? -angle : aRotation.ToEuler().x;
		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(angleTo, 0, 0));

		angleTo = (bRotation.ToEuler().x > angle) ? angle : bRotation.ToEuler().x;
		angleTo = (bRotation.ToEuler().x < -angle) ? -angle : bRotation.ToEuler().x;
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(angleTo, 0, 0));
		break;

	case(Axis::YAW):
		angleTo = (aRotation.ToEuler().y > angle) ? angle : aRotation.ToEuler().y;
		angleTo = (aRotation.ToEuler().y < -angle) ? -angle : aRotation.ToEuler().y;
		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, angleTo, 0));

		angleTo = (bRotation.ToEuler().y > angle) ? angle : bRotation.ToEuler().y;
		angleTo = (bRotation.ToEuler().y < -angle) ? -angle : bRotation.ToEuler().y;
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, angleTo, 0));
		break;
	case(Axis::ROLL):
		angleTo = (aRotation.ToEuler().z > angle) ? angle : aRotation.ToEuler().z;
		angleTo = (aRotation.ToEuler().z < -angle) ? -angle : aRotation.ToEuler().z;
		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, angleTo));

		angleTo = (bRotation.ToEuler().z > angle) ? angle : bRotation.ToEuler().z;
		angleTo = (bRotation.ToEuler().z < -angle) ? -angle : bRotation.ToEuler().z;
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, angleTo));
		break;
	}

	Vector3 offset = (bRotation.ToEuler() - aRotation.ToEuler());
	//Vector3 offset = Vector3(angle, angle, angle) - (bRotation.ToEuler() - aRotation.ToEuler());


	if (abs(offset.x) > 0.01f || abs(offset.y) > 0.01f || abs(offset.z) > 0.01f)
	{
		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 offsetAngle = offset.Normalised();

		Vector3 relativeVelocity = physA->GetAngularVelocity() - physB->GetAngularVelocity();
		float velocityDot = Vector3::Dot(relativeVelocity, offsetAngle);

		float constraintMass = physA->GetInverseMass() + physB->GetInverseMass();

		float biasFactor = 0.5f;
		float bias = -(biasFactor / dt) * offset.Length();

		float lambda = -(velocityDot + bias) / constraintMass;

		Vector3 aImpulse = offsetAngle * lambda * dt;
		Vector3 bImpulse = -offsetAngle * lambda * dt;

		physA->ApplyAngularImpulse(aImpulse);
		physB->ApplyAngularImpulse(bImpulse);
	}
}

//void SingleAxisOrientationConstraint::UpdateConstraint(float dt)
//{
//	Quaternion aRotation = objectA->GetTransform().GetOrientation();
//	Quaternion bRotation = objectB->GetTransform().GetOrientation();
//
//	switch (axis)
//	{
//	case(Axis::PITCH): 
//		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(aRotation.ToEuler().x, 0, 0));
//		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(bRotation.ToEuler().x, 0, 0));
//		break;
//	case(Axis::YAW): 
//		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, aRotation.ToEuler().y, 0));
//		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, bRotation.ToEuler().y, 0));
//		break;
//	case(Axis::ROLL): 
// 		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, aRotation.ToEuler().z));
//		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, bRotation.ToEuler().z));
//		break;
//	}
//
//	Vector3 offset = bRotation.ToEuler() - aRotation.ToEuler();
//	Vector3 offset = Vector3(angle, angle, angle) - (bRotation.ToEuler() - aRotation.ToEuler());
//
//
//	if (abs(offset.x) > 0.01f || abs(offset.y) > 0.01f || abs(offset.z) > 0.01f)
//	{
//		PhysicsObject* physA = objectA->GetPhysicsObject();
//		PhysicsObject* physB = objectB->GetPhysicsObject();
//
//		Vector3 offsetAngle = offset.Normalised();
//
//		Vector3 relativeVelocity = physA->GetAngularVelocity() - physB->GetAngularVelocity();
//		float velocityDot = Vector3::Dot(relativeVelocity, offsetAngle);
//
//		float constraintMass = physA->GetInverseMass() + physB->GetInverseMass();
//
//		float biasFactor = 0.01f;
//		float bias = -(biasFactor / dt) * offset.Length();
//
//		float lambda = -(velocityDot + bias) / constraintMass;
//
//		Vector3 aImpulse = offsetAngle * lambda * dt;
//		Vector3 bImpulse = -offsetAngle * lambda * dt;
//
//		physA->ApplyAngularImpulse(aImpulse);
//		physB->ApplyAngularImpulse(bImpulse);
//	}
//}