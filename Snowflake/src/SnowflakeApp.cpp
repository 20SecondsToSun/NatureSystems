#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SnowflakeApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	float Angle = 0;
	
	size_t LeavesCount = 10;
	float MAX_LEAVES_LEN = 60;

	struct LeaveData
	{
		LeaveData(){};
		LeaveData(float Size) :Size(Size){};
		float Size;
		int Sign = -1;
	};

	std::vector<LeaveData> LeavesLength;

	float MainLineLength = 260;
	size_t RootCount = 6;
	size_t LeavesStep = MainLineLength / LeavesCount;

	float PieAngle = 360 / RootCount;
};

void SnowflakeApp::setup()
{
	setWindowSize(600, 600);


	for (size_t i = 0; i < LeavesCount; i++)
	{
		LeaveData Ld;		
		Ld.Size = Rand::randFloat() > 0.5 ? MAX_LEAVES_LEN : 0;		
		LeavesLength.push_back(Ld);
	}

	//gl::enableAlphaBlending();
	gl::enable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_SMOOTH_LINE_WIDTH_GRANULARITY);
	glEnable(GL_SMOOTH_LINE_WIDTH_RANGE);
	
}

void SnowflakeApp::mouseDown( MouseEvent event )
{
}

void SnowflakeApp::update()
{
}

void SnowflakeApp::draw()
{

	gl::clear( Color( 0, 0, 0 ) ); 	

	gl::pushMatrices();
	gl::translate(getWindowWidth() * 0.5, getWindowHeight() * 0.5);
	gl::rotate(Angle);
	
	for (size_t i = 0; i < RootCount; i++)
	{
		gl::lineWidth(8);
		gl::color(Color::hex(0x5CBCDB));
		gl::rotate(toRadians(PieAngle));
		gl::drawLine(vec2(0, 0), vec2(MainLineLength, 0));		

		gl::pushMatrices();		
		for (size_t j = 0; j < LeavesCount; j++)
		{
			gl::lineWidth(5);	

			gl::pushMatrices();
			gl::rotate(toRadians(30.0));
			gl::drawLine(vec2(0, 0), vec2(0, -LeavesLength[j].Size));
			gl::popMatrices();

			gl::pushMatrices();
			gl::rotate(toRadians(-30.0));
			gl::drawLine(vec2(0, 0), vec2(0, LeavesLength[j].Size));
			gl::popMatrices();

			gl::translate(vec2(LeavesStep, 0));
		}
		gl::popMatrices();
	}

	gl::popMatrices();

	Angle += 0.005;

	for (size_t i = 0; i < LeavesCount; i++)
	{
		LeavesLength[i].Size += 1 * LeavesLength[i].Sign;
		if (LeavesLength[i].Size > MAX_LEAVES_LEN || LeavesLength[i].Size < 0)
			LeavesLength[i].Sign *= -1;
	}
}

CINDER_APP( SnowflakeApp, RendererGl )
