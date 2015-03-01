#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GypnoExpApp : public AppNative {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void circleSlice(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians = false);

	int num;
	float* start_diam;
	float* diam;
	float incr_diam;
	float x, y, sd;
};

void GypnoExpApp::setup()
{
	setWindowSize(700, 700);
	num = 10;
	start_diam = new float[num];
	diam = new float[num];

	x = getWindowWidth() * 0.5;
	y = getWindowHeight() * 0.5;
	incr_diam = getWindowWidth()/num*2;

	sd = 0;
 
	  for (int i=0; i<num; i++) 
	  {
		start_diam[i]= diam[i] = sd;
		sd += incr_diam;
	  }
   
	start_diam[0]= incr_diam * 0.5;

}

void GypnoExpApp::mouseDown( MouseEvent event )
{
}

void GypnoExpApp::update()
{
}

void GypnoExpApp::draw()
{
	// clear out the window with black
	gl::clear( Color::white()); 

	 for (int i=0; i<num; i++) {
    gl::color(Color::white());
   // strokeWeight(40);
    gl::drawStrokedCircle(Vec2f(x, y), diam[i]);
   gl::color(Color::black());
   // strokeWeight(40);
    gl::drawStrokedCircle(Vec2f(x, y), diam[i]);
    diam[i]++;
    if (diam[i]>(start_diam[i]+incr_diam)) diam[i]=start_diam[i];
  }

}


void GypnoExpApp::circleSlice(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians)
{
	int numCirclePts = 100;
	float circlePtsScaled[220];
	//if (!bSetupCircle) setupCircle();  

	// use smoothness, if requested:  
	//if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();  

	bool angleWrap = (lowAngle > highAngle); // are we doing the 0/360 wrap?  

	if(!radians)
	{  
		lowAngle  =  ci::toRadians(lowAngle);  
		highAngle =  ci::toRadians(highAngle);  
	}  

	int res = numCirclePts;  
	float angle = lowAngle;  
	float angleRange = ((!angleWrap)?(highAngle - lowAngle):(M_PI*2 - lowAngle + highAngle));  
	float angleAdder = angleRange / (float)res;  
	int k = 0;  


	for (int i = 0; i < numCirclePts; i++)
	{  
		circlePtsScaled[k]   = x + cos(angle) * radius;  
		circlePtsScaled[k+1] = y - sin(angle) * radius;  
		angle += angleAdder;  
		k+=2;  
	} 
	// we draw the circle points ourself (vs. glDrawArrays) because it allows us to draw the center point, and have the triangles fan around it  
	k = 0;  
	//glBegin((drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : (closed?GL_LINE_LOOP:GL_LINE_STRIP));  
	glBegin(GL_TRIANGLE_FAN);			
	glVertex2f(x, y); // center vertex  

	// now all the points around the circumference  
	for (int i = 0; i < numCirclePts; i++)
	{  
		glVertex2f(circlePtsScaled[k], circlePtsScaled[k+1]);  
		k+=2; 
	}
	glEnd();
}

CINDER_APP_NATIVE( GypnoExpApp, RendererGl )
