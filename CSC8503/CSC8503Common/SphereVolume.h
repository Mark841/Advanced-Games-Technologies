#pragma once
#include "CollisionVolume.h"

namespace NCL {
	class SphereVolume : CollisionVolume
	{
	public:
		SphereVolume(bool hollowSphere = false, float sphereRadius = 1.0f) {
			type	= VolumeType::Sphere;
			hollow = hollowSphere;
			radius	= sphereRadius;
		}
		~SphereVolume() {}

		float GetRadius() const {
			return radius;
		}
		bool GetHollow() const {
			return hollow;
		}
	protected:
		float	radius; 
		bool	hollow;
	};
}

