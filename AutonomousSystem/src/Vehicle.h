#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "FlowField.h"
#include "cinder/Rand.h"
#include "Path.h"

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
		velocity = Vec2d(1,1);
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

	void follow(Path path)
	{
		Vec2d predictVel = velocity;
		predictVel.normalize();
		predictVel *= 10;
		Vec2d predictLoc = location + predictVel;
		Vec2d a = path.getStart();
		Vec2d b = path.getEnd();
		Vec2d normalPoint = getNormalPoint(predictLoc, a, b );

		Vec2d dir = b - a;
		dir.normalize();
		dir *= 5;

		Vec2d target = normalPoint + dir;

		float distance = normalPoint.distance(predictLoc);

		if(distance > path.getRadius())
			seek(target);

	}

	void stayAtBounds(Rectf rect)
	{
		if(!rect.contains(location))
		{
			location = randVec2f();
			location.x *= rect.getWidth();
			location.y *= rect.getHeight();
		}
	}


	float map(float value, 
		float istart, 
		float istop, 
		float ostart, 
		float ostop) {
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}

	Vec2d getNormalPoint(Vec2d p, Vec2d a, Vec2d b) 
	{
		Vec2d ap = p - a; 
		Vec2d ab = b - a;

		ab.normalize();
		ab *= ap.dot(ab);		
		
		return a + ab;
	}


private:
	Vec2d location;
	Vec2d velocity;
	Vec2d acceleration;

	double maxForce;
	double maxSpeed;	
};