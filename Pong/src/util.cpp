#include "../include/util.hpp"

namespace util
{
	Util::Util()
	{
		srand(time(nullptr));
	}
	int Util::randInt(int min, int max)
	{
		return rand() % max + min;
	}
	float Util::randFloat(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
}


