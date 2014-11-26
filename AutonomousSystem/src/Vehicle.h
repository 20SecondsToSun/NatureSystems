#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "FlowField.h"


using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

class Vehicle 
{
public:
	Vehicle(const double x = 0, const double y = 0)
		:velocity(Vec2d::zero()),
		acceleration(Vec2d::zero())

	{
		location = Vec2d(x, y);

		maxSpeed = 10;//Vec2d(40, 40);
		maxForce = 0.3;// Vec2d(0.5, 0.5);
	};

	~Vehicle(){};

	void update() 
	{
		velocity += acceleration;
		velocity.limit(maxSpeed);

		location += velocity;
		acceleration *= 0;
	}

	void draw() 
	{
		gl::color(Color::gray(0.5));
		drawSolidCircle(location, 10, 20);
	}

	void applyForce(Vec2d force)
	{
		acceleration += force;		
	}

	void seek(Vec2d target)
	{
		Vec2d desired = target - location;
		desired.normalize();

		desired *= maxSpeed;		
		Vec2d steer  = desired - velocity;

		steer.limit(maxForce);// = clamp(steer, maxForce);
		applyForce(steer);
	}

	void arrive(Vec2d target)
	{
		Vec2d desired = target - location;		

		float d = desired.length();
		desired.normalize();

		if (d < 100)
		{ 
			float m = map(d, 0, 100, 0, maxSpeed);
			desired *= m;
		} 
		else 
		{
			desired *= maxSpeed;
		}

		Vec2d steer  = desired - velocity;		
		steer.limit(maxForce);
		applyForce(steer);
	}

	void follow(FlowField flow)
	{
		Vec2d desired = flow.lookup(location);
		desired *= maxSpeed;
		Vec2d steer  = desired - velocity;
		steer.limit(maxForce);
		applyForce(steer);
	}


		float map(float value, 
		float istart, 
		float istop, 
		float ostart, 
		float ostop) {
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}


private:
	Vec2d location;
	Vec2d velocity;
	Vec2d acceleration;

	double maxForce;
	double maxSpeed;	
};