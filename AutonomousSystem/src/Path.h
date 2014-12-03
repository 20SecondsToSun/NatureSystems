#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

class Path 
{
public:
	Path()
	{
		radius = 20;
		start  =  Vec2d(0, getWindowHeight()/3);
		end    =  Vec2d(getWindowWidth(),2*getWindowHeight()/3);
	}

	void draw()
	{
		gl::color(Color::white());
		gl::drawLine(start, end);
	}

	Vec2d getStart()
	{
		return start;
	}

	Vec2d getEnd()
	{
		return end;
	}

	float getRadius()
	{
		return radius;
	}


private:
	Vec2d start, end;
	float radius;
};