#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <thread>

namespace generative
{
	struct LineSegment
	{
		LineSegment(ci::Vec2f start, ci::Vec2f end, ci::Color color)
			:start(start), end(end), color(color)
		{

		}

		ci::Vec2f start, end;
		ci::Color color;
	};

	class DLA  
	{
	public:	
		float startRadius, currentRaduis;// = 10;
		std::vector<ci::Vec2f> points;
		float stickDist;
		float pSize;	
		ci::Vec2f center;

		std::vector<LineSegment> lines;

		DLA(int pointsNum)
			:pointsNum(pointsNum),
			startRadius(10),
			stickDist(3),
			pSize(2),
			currentRaduis(10)
		{
			using namespace ci::app;
			using namespace ci;
			center = Vec2f::zero();//Vec2f(getWindowWidth() * 0.5, getWindowHeight() * 0.5);
			points.push_back(center);

			std::thread t([]()
			{
				console() << "thread function\n";
			}
			);
		}

		void update()
		{
			using namespace ci;
			using namespace ci::app;

			float arrayDist, arraySize;
			float maxJumpDist = getWindowWidth();
			int closestElement = 0;

			// spawn a fresh p
			float theta = Rand::randFloat( M_PI * 2 );
			Vec2f freshPoint = center + Vec2f(currentRaduis * sin(theta), currentRaduis * cos(theta));

			// loop until we've plotted a new p
			while (true)
			{
				
				// find the max dist from the array and the closest element
				for (int i = 0; i < points.size(); i++) 
				{
					arrayDist = points[i].distance(freshPoint);

					if (arrayDist < maxJumpDist)
					{
						maxJumpDist = arrayDist;
						closestElement = i;
					}
				}    
			
				// // is the p close enough to stick?
				if (maxJumpDist <= stickDist)
				{
				 // if so, work out where to plot it
				  theta = atan2(freshPoint.y - points[closestElement].y, freshPoint.x - points[closestElement].x);
				  Vec2f addingPoint = points[closestElement] + pSize*  Vec2f(cos(theta), sin(theta));
			
				  // draw that sucka
				 // stroke(((float)points.size()/pointsNum)*255,120,255,120);
				 // line(addingPoint, points[closestElement]);
				  Color color = Color(((float)points.size()/pointsNum), 120/255.0f, 1.0f);//, 120/255.0f);
				  lines.push_back(LineSegment(addingPoint, points[closestElement], color));


				  // adjust our p start radius, increment and either stop or bust outta here
				  for (int i=0; i < points.size(); i++) 
				  {
					arraySize = points[i].distance(Vec2f::zero());

					if (arraySize > currentRaduis)					
					  currentRaduis = arraySize;					
				  }

				  points.push_back(addingPoint);

				 // if (pCount == pointsNum)
					//noLoop();
				  
				  break;
				}

				// move p
				theta = Rand::randFloat( M_PI * 2 );
				freshPoint += Vec2f(Rand::randFloat(maxJumpDist)*sin(theta), Rand::randFloat(maxJumpDist)*cos(theta));
				
				if (freshPoint.distance(Vec2f::zero()) > currentRaduis * 2)				
					freshPoint = Vec2f(currentRaduis*sin(theta), currentRaduis*cos(theta));					
			}
		}

		void draw()
		{
			using namespace ci;
			using namespace ci::app;

			gl::pushMatrices();
			gl::translate(Vec2f(getWindowWidth() * 0.5, getWindowHeight() * 0.5));

			for(auto line : lines)
			{
				gl::color(line.color);
				gl::drawLine(line.start, line.end);
			}

			gl::popMatrices();
		}

	private:
		int pointsNum;


	};	
}