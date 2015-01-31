#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Tree.h"
#include "Tree3d.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpaceColonizationApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );		
	void mouseDrag( MouseEvent event );
	void update();
	void draw();

	Tree *tree;
	tree3d::Tree3d *treeVol;

	params::InterfaceGlRef	mParams;
	void button();

	string		treeMode;
	ci::MayaCamUI					mMayaCam;
};

void SpaceColonizationApp::setup()
{
	setWindowSize(1000, 800);
	tree = new Tree(Vec2f(500.0f, 800.0f));
	treeVol = new tree3d::Tree3d(Vec3f(0.0f, 0.0f, 0.0f));

	treeMode = "3D";

	// Create the interface and give it a name.
	mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( Vec2i( 200, 400 ) ) );

	if (treeMode == "3D")
	{
		mParams->addParam( "LeafCount", &treeVol->LeafCount ).min( 100 ).max( 1500).step( 100 );
		mParams->addParam( "branchLength", &treeVol->branchLength ).min( 1 ).max( 10).step( 1 );
		mParams->addParam( "TreeWidth", &treeVol->TreeWidth ).min( 100 ).max( 1000).step( 10 );
		mParams->addParam( "TreeHeight", &treeVol->TreeHeight ).min( 100 ).max( 1000).step( 10 );
		mParams->addParam( "MinDistance", &treeVol->MinDistance ).min( 1 ).max( 10).step( 1 );
		mParams->addParam( "MaxDistance", &treeVol->MaxDistance ).min( 10 ).max( 100).step( 5 );
	}
	else
	{
		mParams->addParam( "LeafCount", &tree->LeafCount ).min( 100 ).max( 1500).step( 100 );
		mParams->addParam( "branchLength", &tree->branchLength ).min( 1 ).max( 10).step( 1 );
		mParams->addParam( "TreeWidth", &tree->TreeWidth ).min( 100 ).max( 1000).step( 10 );
		mParams->addParam( "TreeHeight", &tree->TreeHeight ).min( 100 ).max( 1000).step( 10 );
		mParams->addParam( "MinDistance", &tree->MinDistance ).min( 1 ).max( 10).step( 1 );
		mParams->addParam( "MaxDistance", &tree->MaxDistance ).min( 10 ).max( 100).step( 5 );
	}
	
 


	// Other types of controls that can be added to the interface
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
	
	if(treeMode == "3D")
	{
		treeVol->clear();
		treeVol->generate();
	}
	else if(treeMode == "2D")
	{
		tree->clear();
		tree->generate();
	}
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
	if(treeMode == "3D")
	{
		treeVol->grow();
	}
	else if(treeMode == "2D")
	{
		tree->grow();
	}
	
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

		treeVol->draw();
		gl::drawSphere(Vec3f(0, 0, 0), 5.5, 16);

		gl::disableDepthRead();
		gl::disableDepthWrite();

		gl::popMatrices();
	}
	else if(treeMode == "2D")
	{
		tree->draw();
	}

	
	mParams->draw();
}



CINDER_APP_NATIVE( SpaceColonizationApp, RendererGl )
