#include "GameObject.h"
#include "SingleAxisOrientationConstraint.h"

using namespace NCL::CSC8503;

void SingleAxisOrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();

	switch (axis)
	{
	case(Axis::PITCH): 
		if (aRotation.ToEuler().x > angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(angle, 0, 0));
		}
		else if (aRotation.ToEuler().x < -angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-angle, 0, 0));
		}
		else
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(aRotation.ToEuler().x, 0, 0));
		}

		if (bRotation.ToEuler().x > angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(angle, 0, 0));
		}
		else if (bRotation.ToEuler().x < -angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-angle, 0, 0));
		}
		else
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(bRotation.ToEuler().x, 0, 0));
		}
		break;
	case(Axis::YAW):
		if (aRotation.ToEuler().y > angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, angle, 0));
		}
		else if (aRotation.ToEuler().y < -angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -angle, 0));
		}
		else
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, aRotation.ToEuler().y, 0));
		}

		if (bRotation.ToEuler().y > angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, angle, 0));
		}
		else if (bRotation.ToEuler().y < -angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -angle, 0));
		}
		else
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, bRotation.ToEuler().y, 0));
		}
		break;
	case(Axis::ROLL):
		if (aRotation.ToEuler().z > angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, angle));
		}
		else if (aRotation.ToEuler().z < -angle)
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, -angle));
		}
		else
		{
			objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, aRotation.ToEuler().z));
		}

		if (bRotation.ToEuler().z > angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, angle));
		}
		else if (bRotation.ToEuler().z < -angle)
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, -angle));
		}
		else
		{
			objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, bRotation.ToEuler().z));
		}
		break;
	}

	Vector3 offset = bRotation.ToEuler() - aRotation.ToEuler();


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
}/*
void SingleAxisOrientationConstraint::UpdateConstraint(float dt)
{
	Quaternion aRotation = objectA->GetTransform().GetOrientation();
	Quaternion bRotation = objectB->GetTransform().GetOrientation();

	switch (axis)
	{
	case(Axis::PITCH): 
		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(aRotation.ToEuler().x, 0, 0));
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(bRotation.ToEuler().x, 0, 0));
		break;
	case(Axis::YAW): 
		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, aRotation.ToEuler().y, 0));
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, bRotation.ToEuler().y, 0));
		break;
	case(Axis::ROLL): 
 		objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, aRotation.ToEuler().z));
		objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, bRotation.ToEuler().z));
		break;
	}

	Vector3 offset = bRotation.ToEuler() - aRotation.ToEuler();


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
}*/