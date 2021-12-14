#include "GameObject.h"
#include "FacingConstraint.h"
#include <cmath>
#define PI 3.14159265

using namespace NCL::CSC8503;

void FacingConstraint::UpdateConstraint(float dt)
{
	Vector3 pos = object->GetTransform().GetPosition();
	Vector3 relativePos = origin - pos;
	Vector3 orient = pos - origin;

	if (orient.z != 0)
	{
		if (orient.y != 0)
		{
			xAngle = atan(orient.y / orient.z) * 180 / PI;
		}
		else
		{
			xAngle = 0;
		}
		if (orient.x != 0)
		{
			yAngle = atan(orient.x / orient.z) * 180 / PI;
		}
		else
		{
			yAngle = 0;
		}
	}
	else
	{
		xAngle = 0;
		yAngle = 0;
	}

	Vector3 o = object->GetTransform().GetOrientation().ToEuler();

	object->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-xAngle, yAngle, 0));
	Vector3 o1 = object->GetTransform().GetOrientation().ToEuler();
}