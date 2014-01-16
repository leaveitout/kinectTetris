#include "testApp.h"

ofImage depth_image;
ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_NOTICE);
	//ofBackground(0);
	ofBackgroundHex(0xfdefc2);
	
	device.setup();
	
	// OpenNI initialisation
	if (tracker.setup(device))
	{
		ofLog(OF_LOG_NOTICE, "OpenNI tracker initialized");
		//cout << "tracker inited" << endl;
	}

	// Box2d initilisation
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createBounds();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
}

void testApp::exit()
{
	tracker.exit();
	device.exit();
}

//--------------------------------------------------------------
void testApp::update()
{
	device.update();

	box2d.update(); 
}

//--------------------------------------------------------------
void testApp::draw()
{
	// draw depth
	depth_image.setFromPixels(tracker.getPixelsRef(1000, 4000));
	
	ofSetColor(255);
	depth_image.draw(0, 0);
	
	// draw in 2D
	ofPushView();
	tracker.getOverlayCamera().begin(ofRectangle(0, 0, depth_image.getWidth(), depth_image.getHeight()));
	ofDrawAxis(100);
	tracker.draw();
	tracker.getOverlayCamera().end();
	ofPopView();
	
	// draw in 3D
	cam.begin();
	ofDrawAxis(100);
	tracker.draw();
	
	// draw box
	ofNoFill();
	ofSetColor(255, 0, 0);
	for (int i = 0; i < tracker.getNumUser(); i++)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);
		const ofxNiTE2::Joint &joint = user->getJoint(nite::JOINT_HEAD);
		
		joint.transformGL();
		ofBox(300);
		joint.restoreTransformGL();
	}
	
	cam.end();

	for(int i=0; i<circles.size(); i++) {
                ofFill();
                ofSetHexColor(0xf6c738);
                circles[i].get()->draw();
        }
        
        for(int i=0; i<boxes.size(); i++) {
                ofFill();
                ofSetHexColor(0xBF2545);
                boxes[i].get()->draw();
        }

        // draw the ground
        box2d.drawGround();
        
        
        
        string info = "";
        info += "Press [c] for circles\n";
        info += "Press [b] for blocks\n";
        info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
        info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
        info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

	if(key == 'c') {
                float r = ofRandom(4, 20);
                circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
                circles.back().get()->setPhysics(3.0, 0.53, 0.1);
                circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
                
        }
        
        if(key == 'b') {
                float w = ofRandom(20, 60);
                float h = ofRandom(20, 60);
                boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
                boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
                boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
        }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}