#include "Math.h"

float fe::math::randomClamped()
{
	return  (static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / 2))) - 1.0;
}