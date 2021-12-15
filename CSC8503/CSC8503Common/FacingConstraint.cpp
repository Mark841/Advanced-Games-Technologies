#include "GameObject.h"
#include "FacingConstraint.h"
#include <cmath>
#define PI 3.14159265

using namespace NCL::CSC8503;

void FacingConstraint::UpdateConstraint(float dt)
{
	if (objectB != nullptr)
	{
		origin = objectB->GetTransform().GetPosition();
	}
	Vector3 pos = objectA->GetTransform().GetPosition();
	Vector3 relativePos = origin - pos;
	Vector3 orient = pos - origin;

	float xAngle;
	float yAngle;

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

	objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(-xAngle, yAngle, 0));
}