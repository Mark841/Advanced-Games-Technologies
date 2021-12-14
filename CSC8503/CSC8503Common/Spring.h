#pragma once

		class Spring
		{
		public:
			Spring(float length, float snappiness, float damping)
			{
				this->length = length;
				this->k = snappiness;
				this->damping = damping;
			}

			float GetLength() const { return length; }
			float GetSnappiness() const { return k; }
			float GetDamping() const { return damping; }

		protected:
			float length;
			float k;
			float damping;
		};