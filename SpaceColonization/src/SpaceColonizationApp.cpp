#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Tree2D.h"
#include "Tree3D.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "ITRee.h"
#include "cinder/TriMesh.h"
#include "MeshHelper.h"
#include "cinder/gl/Vbo.h"

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

	params::InterfaceGlRef	mParams;
	ci::MayaCamUI* mMayaCam;

	ITree *tree;
	TriMesh mesh;
	std::vector<Vec3f> vecs ;
};

void SpaceColonizationApp::setup()
{
	setWindowSize(1000, 800);	

	mParams = ci::params::InterfaceGl::create( getWindow(), "App parameters", toPixels( Vec2i( 200, 400 ) ) );	
	mParams->addParam( "LeafCount", &tree->leafCount ).min( 100 ).max( 1500).step( 100 );
	mParams->addParam( "branchLength", &tree->branchLength ).min( 1 ).max( 10).step( 1 );
	mParams->addParam( "TreeWidth", &tree->treeWidth ).min( 100 ).max( 1000).step( 10 );
	mParams->addParam( "TreeHeight", &tree->treeHeight ).min( 100 ).max( 1000).step( 10 );
	mParams->addParam( "MinDistance", &tree->minDistance ).min( 1 ).max( 200).step( 1 );
	mParams->addParam( "MaxDistance", &tree->maxDistance ).min( 10 ).max( 100).step( 5 );
	mParams->addButton( "Generate!", std::bind( &SpaceColonizationApp::button, this ) );

	// set up the camera
	CameraPersp cam;
	cam.setEyePoint( Vec3f(0.0f, 300.0f, 1000.0f) );
	cam.setCenterOfInterestPoint( Vec3f(0.0f, 0.0f, 0.0f) );
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 1.0f, 5000.0f );

	mMayaCam = new ci::MayaCamUI();

	mMayaCam->setCurrentCam(cam);

	mesh	= MeshHelper::createTorus(Vec2f(80, 80), 0.5f);
	vecs    = mesh.getVertices();
	for (int i = 0; i < vecs.size(); i++)		
		vecs[i] = vecs[i] * 1000;

	tree = new Tree3D(mMayaCam, Vec3f::zero(), vecs);
	//tree = new Tree2D(Vec2f(500, 500));
	tree->runInThread();

	//Vec3f v0 = Vec3f( -200, -100, 0 );
	//Vec3f v1 = Vec3f( 200, -100, 0 );
	//Vec3f v2 = Vec3f( 0, 200, 0 );

	////Pyramid's top
	//Vec3f v3 = Vec3f( 0, 0, 100 );
	////Add triangles by its 
	//

	//mesh.appendVertex( v3 );
	//mesh.appendVertex( v2 ); 
	//mesh.appendVertex( v0 );

	//mesh.appendVertex( v3 ); 
	//mesh.appendVertex( v1 ); 
	//mesh.appendVertex( v2 );

	//mesh.appendVertex( v3 ); 
	//mesh.appendVertex( v0 );
	//mesh.appendVertex( v1 );

	//mesh.

}

void SpaceColonizationApp::button()
{	
	tree->clear();
	tree->generate();
}

void SpaceColonizationApp::mouseDown( MouseEvent event )
{
	mMayaCam->mouseDown( event.getPos() );	
}

void SpaceColonizationApp::mouseDrag( MouseEvent event )
{	
	mMayaCam->mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void SpaceColonizationApp::update()
{	
//	tree->grow();	
}

void SpaceColonizationApp::draw()
{
	gl::clear( Color::black()); 

	tree->draw();
	mParams->draw();
}
CINDER_APP_NATIVE( SpaceColonizationApp, RendererGl )