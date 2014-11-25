#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AutonomousSystemApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void AutonomousSystemApp::setup()
{
}

void AutonomousSystemApp::mouseDown( MouseEvent event )
{
}

void AutonomousSystemApp::update()
{
}

void AutonomousSystemApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( AutonomousSystemApp, RendererGl )
