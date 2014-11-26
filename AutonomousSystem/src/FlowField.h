#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/Perlin.h"


using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

class FlowField 
{
public:
	FlowField(int _resolution)
	{
		resolution = _resolution;

		cols = getWindowWidth() / _resolution;
		rows = getWindowHeight() / _resolution;

		// allocation
		/*field = new Vec2d*[rows];
		for(int i = 0; i < rows; i++)
		field[i] = new Vec2d[cols];*/

		init();
	}

	void init() 
	{
		mPerlin = Perlin();

		float xoff = 0;
		for (int i = 0; i < cols; i++) 
		{
			float yoff =0;
			for (int j = 0; j <rows; j++) 
			{
				float noise = mPerlin.fBm( Vec2f(xoff,yoff));

				float theta = map(noise, 0, 1, 0, 2*3.1415);

				//field[i][j] = Vec2d(cos(theta),sin(theta));
				field.push_back(Vec2d(cos(theta),sin(theta)));
				yoff += 0.1;
			}
			xoff += 0.1;

		}
	}

	Vec2d lookup(Vec2d lookup) 
	{
		int column = constrain((int)lookup.x/resolution,0,cols-1);
		int row = constrain((int)lookup.y/resolution,0,rows-1);
		console()<<column<<endl;
		return field[column*row];
	}

	float map(float value, 
		float istart, 
		float istop, 
		float ostart, 
		float ostop) {
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}

private:
	int resolution;
	int cols, rows;

	vector<Vec2d> field;

	Perlin mPerlin;
};