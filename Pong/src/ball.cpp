#include "../include/ball.hpp"
namespace pong
{
	Ball::Ball(util::Util utl)
	{
		x = 0;
		y = 0;
		vx = 0;
		vy = 0;
		util = utl;
	}

	void Ball::start()
	{
		vx = util.randFloat(-0.0025f, -0.0075f);
		vy = util.randFloat(-0.005f, -0.005f);
	}

	void Ball::reset()
	{
		x = 0;
		y = 0;
		start();
	}

	void Ball::tick()
	{
		x += vx;
		y += vy;
	}

	void Ball::collideUp()
	{
		if (vy > 0)
		{
			vy = -(vy - util.randFloat(-0.001f, -0.005f));
		}
		else
		{
			vy = util.randFloat(-0.005f, -0.001f);
		}
	}

	void Ball::collideDown()
	{
		if (vy < 0)
		{
			vy = -(vy + util.randFloat(0.001f, 0.005f));
		}
		else
		{
			vy = util.randFloat(0.001f, 0.005f);
		}
	}

	void Ball::collideRight()
	{
		if (vx < 0)
		{
			vx = (vx + 0.0040);
			vx = -(vx + util.randFloat(-0.0025f, -0.0075f));
		}
		else
		{
			vx = util.randFloat(-0.025f, 0.025f);
		}
	}

	void Ball::collideLeft()
	{
		if (vx > 0)
		{
			vx = (vx - 0.0040);
			vx = -(vx - util.randFloat(-0.0025f, -0.0075f));
		}
		else
		{
			vx = -util.randFloat(-0.0025f, -0.0075f);
		}
		vy += util.randFloat(-0.025f, 0.025f);
	}

}

