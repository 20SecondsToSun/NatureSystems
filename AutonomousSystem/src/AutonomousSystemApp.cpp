#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Vehicle.h"
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
};

void AutonomousSystemApp::setup()
{
	vec = new Vehicle(50, 50);
	i = 0;
	mRunning = true;
	mThread		= ThreadRef( new thread( &AutonomousSystemApp::run, this ) );
}

void AutonomousSystemApp::run( )
{
	while(mRunning)
	console()<<i++<<endl;
}

void AutonomousSystemApp::mouseDown( MouseEvent event )
{
	vec->seek(event.getPos());
}

void AutonomousSystemApp::update()
{
	vec->update();
}

void AutonomousSystemApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	vec->draw();
}

void AutonomousSystemApp::shutdown()
{
	mRunning = false;
	if ( mThread ) {
		console()<<"::::::::::::::::::"<<endl;
		mThread->join();
		mThread.reset();
	}
}

CINDER_APP_NATIVE( AutonomousSystemApp, RendererGl )
