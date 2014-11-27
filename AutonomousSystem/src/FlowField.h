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
		fieldArr = new Vec2d*[rows];
		for(int i = 0; i < rows; i++)
			fieldArr[i] = new Vec2d[cols];

		
		init();
	}

	void init() 
	{
		mPerlin = Perlin();

		float xoff = 0;
		for (int i = 0; i < rows; i++) 
		{
			float yoff =0;
			for (int j = 0; j <cols; j++) 
			{
				float noise = mPerlin.noise( xoff, yoff);
				float theta = map(noise, 0, 1, 0, 2*3.1415);

				fieldArr[i][j] = Vec2d(cos(theta),sin(theta));
				field.push_back(Vec2d(cos(theta),sin(theta)));
				yoff += 0.1;
			}
			xoff += 0.1;
		}
	}

	Vec2d lookup(Vec2d lookup) 
	{
		
		int column = constrain((int)lookup.x/resolution,0,cols-1);
		int row    = constrain((int)lookup.y/resolution,0,rows-1);		
		return fieldArr[row][column];
	}

	void drawVectorField()
	{
		drawGrid();
		drawVectors();
	}

private:
	int resolution;
	int cols, rows;

	vector<Vec2d> field;
	Vec2d **fieldArr;
	Perlin mPerlin;

	inline float map(float value, float istart, float istop, float ostart, float ostop)
	{
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}

	void drawGrid()
	{
		gl::lineWidth(0.01f);

		for (int i = 0; i < rows; i++) 
		{
			float yPos = i * resolution;
			gl::drawLine(Vec2f(0.0f, yPos), Vec2f(getWindowWidth(), yPos));
		}

		for (int i = 0; i < cols; i++) 
		{
			float xPos = i * resolution;
			gl::drawLine(Vec2f(xPos, 0.0f), Vec2f(xPos, getWindowHeight()));
		}
	}

	void drawVectors()
	{
		for (int i = 0; i < rows; i++) 
		{			
			for (int j = 0; j <cols; j++) 
			{
				//fieldArr[i][j]
			}
		}
	}
};