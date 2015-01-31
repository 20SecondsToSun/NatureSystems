#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Tree2D.h"
#include "Tree3D.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "ITRee.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace spacecolonization3D;

class SpaceColonizationApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );		
	void mouseDrag( MouseEvent event );
	void update();
	void draw();	
	void button();

	ITree *tree;

	params::InterfaceGlRef	mParams;
	string		treeMode;
	ci::MayaCamUI mMayaCam;
};

void SpaceColonizationApp::setup()
{
	setWindowSize(1000, 800);

	tree = new Tree3D(Vec3f::zero());
	//tree = new Tree2D(Vec2f(500, 500));
	tree->runInThread();
	treeMode = "3D";

	mParams = ci::params::InterfaceGl::create( getWindow(), "App parameters", toPixels( Vec2i( 200, 400 ) ) );	
	mParams->addParam( "LeafCount", &tree->leafCount ).min( 100 ).max( 1500).step( 100 );
	mParams->addParam( "branchLength", &tree->branchLength ).min( 1 ).max( 10).step( 1 );
	mParams->addParam( "TreeWidth", &tree->treeWidth ).min( 100 ).max( 1000).step( 10 );
	mParams->addParam( "TreeHeight", &tree->treeHeight ).min( 100 ).max( 1000).step( 10 );
	mParams->addParam( "MinDistance", &tree->minDistance ).min( 1 ).max( 10).step( 1 );
	mParams->addParam( "MaxDistance", &tree->maxDistance ).min( 10 ).max( 100).step( 5 );
	mParams->addButton( "Generate!", std::bind( &SpaceColonizationApp::button, this ) );

	// set up the camera
	CameraPersp cam;
	cam.setEyePoint( Vec3f(0.0f, 300.0f, 1500.0f) );
	cam.setCenterOfInterestPoint( Vec3f(0.0f, 0.0f, 0.0f) );
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 1.0f, 5000.0f );
	mMayaCam.setCurrentCam( cam );
}

void SpaceColonizationApp::button()
{	
	tree->clear();
	tree->generate();
}

void SpaceColonizationApp::mouseDown( MouseEvent event )
{
	mMayaCam.mouseDown( event.getPos() );	
}

void SpaceColonizationApp::mouseDrag( MouseEvent event )
{	
	mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void SpaceColonizationApp::update()
{	
	//tree->grow();	
}

void SpaceColonizationApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	if(treeMode == "3D")
	{	
		gl::pushMatrices();
		gl::setMatrices( mMayaCam.getCamera() );

		gl::enableDepthRead();
		gl::enableDepthWrite();

		tree->draw();
		gl::drawSphere(Vec3f(0, 0, 0), 5.5, 16);

		gl::disableDepthRead();
		gl::disableDepthWrite();

		gl::popMatrices();
	}
	else if(treeMode == "2D")	
		tree->draw();	
	
	mParams->draw();
}
CINDER_APP_NATIVE( SpaceColonizationApp, RendererGl )