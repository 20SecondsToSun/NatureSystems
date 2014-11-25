#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpaceColonizationApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void SpaceColonizationApp::setup()
{
}

void SpaceColonizationApp::mouseDown( MouseEvent event )
{
}

void SpaceColonizationApp::update()
{
}

void SpaceColonizationApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( SpaceColonizationApp, RendererGl )
