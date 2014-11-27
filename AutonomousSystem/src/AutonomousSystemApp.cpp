#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Vehicle.h"
#include "FlowField.h"
#include <thread>
typedef std::shared_ptr<std::thread> ThreadRef;
using namespace ci;
using namespace ci::app;
using namespace std;

class AutonomousSystemApp : public AppNative {
  public:
	

	void setup();
	void mouseDown( MouseEvent event );	

	void update();
	void draw();


	Vehicle*  vec;

	void run( );
	void shutdown();
	ThreadRef							mThread;
	bool mRunning ;
	int i;

	Vec2d point;
	FlowField *flowField;
};

void AutonomousSystemApp::setup()
{
	vec = new Vehicle(200, 200);
	flowField = new FlowField(20);
	
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
	//vec->arrive(point);
	vec->follow(*flowField);

	vec->update();
}

void AutonomousSystemApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	flowField->drawVectorField();
	vec->draw();

}

void AutonomousSystemApp::shutdown()
{

}

CINDER_APP_NATIVE( AutonomousSystemApp, RendererGl )
