#pragma once

#include "ofMain.h"

#include "ofxNI2.h"
#include "ofxNiTE2.h"

#include "ofxBox2d.h"

#include "TextureShape.h"
#include "HandObject.h"

class testApp : public ofBaseApp
{
public:
	void setup();
	void exit();
	
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxNI2::Device device;
	ofxNiTE2::UserTracker tracker;

private:
	ofImage depthImage;
	ofImage colorImage;

	ofxNI2::ColorStream color;

	ofVec3f leftHandJointPositionScreen;
	ofVec3f rightHandJointPositionScreen;

	bool drawDepth;
	bool drawTracker;
	bool drawDebug;

	ofxBox2d							box2d;
	vector <ofPtr<ofxBox2dCircle> >		circles;     // default box2d circles
	vector <ofPtr<ofxBox2dRect> >		boxes;       // defalut box2d rects

	ofPtr<ofxBox2dRect>					leftHandBox;
	ofPtr<ofxBox2dRect>					rightHandBox;

	ofRectangle viewport;

	double currTime;

	// a vector of all the texture images
    vector <ofImage> textures;
	vector <float>	 densities;

    // a list of all the texture shapes
    list <ofPtr<TextureShape> > shapes;

	// Hand box2d objects
	HandObject leftHandObject;
	HandObject rightHandObject;

	// TODO: max this a const
	float newObjectTimeMax;
	float currTimef;
	float prevTimef;
	float newObjectTimeCounter;
};