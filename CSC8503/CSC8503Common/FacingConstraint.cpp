#include "GameObject.h"
#include "FacingConstraint.h"
#include <cmath>
#define PI 3.14159265

using namespace NCL::CSC8503;

void FacingConstraint::UpdateConstraint(float dt)
{
	Vector3 pos = object->GetTransform().GetPosition();
	Vector3 relativePos = origin - pos;

	float xAngle = atan((pos.x - origin.x) / (pos.z - origin.z)) * 180/PI;
	float yAngle = atan((pos.y- origin.y) / (pos.z - origin.z)) * 180 / PI;
	float zAngle = atan((pos.z - origin.z) / (pos.x - origin.x)) * 180 / PI;

	Vector3 o = object->GetTransform().GetOrientation().ToEuler();

	object->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(xAngle, yAngle, zAngle));
	Vector3 o1 = object->GetTransform().GetOrientation().ToEuler();
}