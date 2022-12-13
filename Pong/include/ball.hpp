#ifndef BALL_H
#define BALL_H

#include "util.hpp"

namespace pong
{
	class Ball
	{
	public:
		float x, y, vx, vy;
		util::Util util;
		Ball(util::Util utl);
		void start();
		void reset();
		void tick();
		void collideUp();
		void collideDown();
		void collideRight();
		void collideLeft();
	};
}


#endif
