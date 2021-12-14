#pragma once

class Spring
{
public:
	Spring(float length, float snappiness)
	{
		this->length = length;
		this->k = snappiness;
	}
	float GetLength() const { return length; }
	float GetSnappiness() const { return k; }

protected:
	float length;
	float k;
};