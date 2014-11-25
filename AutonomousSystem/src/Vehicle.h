#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

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
		console()<<"acceleration::  "<<acceleration<<endl;
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

private:
	Vec2d location;
	Vec2d velocity;
	Vec2d acceleration;

	double maxForce;
	double maxSpeed;	
};