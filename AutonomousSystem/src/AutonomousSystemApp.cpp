#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Vehicle.h"
#include "FlowField.h"
#include "Path.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AutonomousSystemApp : public AppNative
{
  public:

	void setup();
	void mouseDown( MouseEvent event );	

	void update();
	void draw();

	Vehicle*  vec;
	void run( );
	void shutdown();


	Vec2d point;
	FlowField *flowField;
	Path *path;
};

void AutonomousSystemApp::setup()
{
	vec = new Vehicle(200, 100);
	flowField = new FlowField(20);
	path = new Path();
}

void AutonomousSystemApp::run( )
{

}

void AutonomousSystemApp::mouseDown( MouseEvent event )
{
	point = event.getPos();
}

void AutonomousSystemApp::update()
{
	vec->arrive(point);
	vec->follow(*flowField);
	
	vec->follow(*path);
	vec->update();
	vec->stayAtBounds(getWindowBounds());
}

void AutonomousSystemApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	flowField->drawVectorField();
	//path->draw();
	vec->draw();

}

void AutonomousSystemApp::shutdown()
{

}

CINDER_APP_NATIVE( AutonomousSystemApp, RendererGl )
