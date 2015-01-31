#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "DLA.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace generative;

class DLAApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	DLA *dla;
};

void DLAApp::setup()
{
	dla = new DLA(6000);
}

void DLAApp::mouseDown( MouseEvent event )
{
}

void DLAApp::update()
{
	dla->update();
}

void DLAApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	dla->draw();
}

CINDER_APP_NATIVE( DLAApp, RendererGl )
