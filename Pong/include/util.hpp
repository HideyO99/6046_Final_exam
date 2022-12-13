#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <ctime>

namespace util
{
	class Util
	{
	public:
		Util();
		float randFloat(float min, float max);
		int randInt(int min, int max);
	};
}

#endif
