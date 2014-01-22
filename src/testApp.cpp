#include "testApp.h"


ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	//ofDisableAntiAliasing();
	ofSetLogLevel(OF_LOG_NOTICE);
	ofBackground(0);
	//ofBackgroundHex(0xfdefc2);

	viewport = ofRectangle(0.0F, 0.0F, ofGetWidth(), ofGetHeight());
	
	device.setup();

	// OpenNI initialisation
	if (tracker.setup(device))
	{
		ofLog(OF_LOG_NOTICE, "OpenNI tracker initialized");
	}

	color.setup(device);
	color.start();

	drawDepth = false;
	drawTracker = false;
	drawDebug = false;

	// Flip the color image
	if(!color.getMirror())
		color.setMirror(true);

	if(!color.getAutoExposureEnabled())
		color.setAutoExposureEnabled(true);

	if(!color.getAutoWhiteBalanceEnabled())
		color.setAutoWhiteBalanceEnabled(true);

	if(device.isRegistrationSupported())
		device.setEnableRegistration();

	// find all the texture files and load them
    ofDirectory dir;
    ofDisableArbTex();
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        textures.push_back(ofImage(dir.getPath(i)));
    }
    printf("%i Textures Loaded\n", (int)textures.size());

	// TODO: Use xml settings
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);
	densities.push_back(3.0F);

	newObjectTimeCounter = 0.0;
	newObjectTimeMax = 4.0F;
	currTimef = 0.0F;
	prevTimef = 0.0F;

	leftHandClicked = false;
	rightHandClicked = false;

	// Box2d initilisation
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createBounds(-(float)ofGetWidth(), -(float)ofGetHeight(), 3.0F*(float)ofGetWidth(), 3.0F*(float)ofGetHeight());
    box2d.setFPS(60.0);
    box2d.registerGrabbing();

	leftHandObject.setPhysics(5.0, 0.25, 0.5);
	leftHandObject.enableGravity(false);
	rightHandObject.setPhysics(5.0, 0.25, 0.5);
	rightHandObject.enableGravity(false);

	leftHandObject.setup(box2d.getWorld(), 0.0F, 0.0F, 100.0F, 30.0F);
	//leftHandObject.setup(box2d.getWorld(), 0.0F, -(float)ofGetHeight(), 100.0F, 30.0F);
	rightHandObject.setup(box2d.getWorld(), ofGetWidth()/2.0F, -(float)ofGetHeight(), 100.0F, 30.0F);
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

	/// TODO: Add some tracked person management
	///
	if(tracker.getNumUser() != 0)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(tracker.getNumUser()-1);

		// Left Hand
		const ofxNiTE2::Joint &leftHandJoint = user->getJoint(nite::JOINT_LEFT_HAND);

		// TODO: Add in confidence checking
		float confidence = leftHandJoint.getPositionConfidence();

		// TODO: Add in joint orientation information
		if(confidence >= 0.5F)
		{
			nite::SkeletonJoint leftHandJointNite = leftHandJoint.get();

			//nite::Quaternion leftHandRotation = leftHandJointNite.getOrientation();
			ofVec3f leftHandJointPosition(leftHandJointNite.getPosition().x, leftHandJointNite.getPosition().y, leftHandJointNite.getPosition().z);
			leftHandJointPositionScreen = tracker.getOverlayCamera().worldToScreen(leftHandJointPosition, viewport);
		
			// Need to move origin
			leftHandJointPositionScreen = ofVec3f(	ofGetWidth() - leftHandJointPositionScreen.x,
													ofGetHeight() - leftHandJointPositionScreen.y,
													leftHandJointPositionScreen.z	);

			float quarterWidth = leftHandObject.getWidth()/8.0F;

			if(!leftHandClicked)
			{
				leftHandObject.setPosition(	leftHandJointPositionScreen.x , 
										leftHandJointPositionScreen.y);

				box2d.grabShapeDown(leftHandJointPositionScreen.x - quarterWidth, leftHandJointPositionScreen.y, 0);
				box2d.grabShapeDown(leftHandJointPositionScreen.x + quarterWidth, leftHandJointPositionScreen.y, 1);

				leftHandClicked = true;
			}
			else
			{
				box2d.grabShapeDragged(leftHandJointPositionScreen.x - quarterWidth, leftHandJointPositionScreen.y, 0);
				box2d.grabShapeDragged(leftHandJointPositionScreen.x + quarterWidth, leftHandJointPositionScreen.y, 1);
			}
		}

		// RightHand
		const ofxNiTE2::Joint &rightHandJoint = user->getJoint(nite::JOINT_RIGHT_HAND);

		confidence = leftHandJoint.getPositionConfidence();

		if(confidence >= 0.5F)
		{
			nite::SkeletonJoint rightHandJointNite = rightHandJoint.get();
			ofVec3f rightHandJointPosition(rightHandJointNite.getPosition().x, rightHandJointNite.getPosition().y, rightHandJointNite.getPosition().z);
			rightHandJointPositionScreen = tracker.getOverlayCamera().worldToScreen(rightHandJointPosition, viewport);

			// Need to move origin
			rightHandJointPositionScreen = ofVec3f(	ofGetWidth() - rightHandJointPositionScreen.x,
													ofGetHeight() - rightHandJointPositionScreen.y,
													rightHandJointPositionScreen.z	);

			float quarterWidth = rightHandObject.getWidth()/8.0F;

			if(!rightHandClicked)
			{
				rightHandObject.setPosition(	rightHandJointPositionScreen.x , 
										rightHandJointPositionScreen.y);

				box2d.grabShapeDown(rightHandJointPositionScreen.x - quarterWidth, rightHandJointPositionScreen.y, 2);
				box2d.grabShapeDown(rightHandJointPositionScreen.x + quarterWidth, rightHandJointPositionScreen.y, 3);

				rightHandClicked = true;
			}
			else
			{
				box2d.grabShapeDragged(rightHandJointPositionScreen.x - quarterWidth, rightHandJointPositionScreen.y, 2);
				box2d.grabShapeDragged(rightHandJointPositionScreen.x + quarterWidth, rightHandJointPositionScreen.y, 3);
			}
		}
	}

	/// TODO: Use shouldRemoveOffScreen function instead
	list<ofPtr<TextureShape>>::iterator it = shapes.begin(); 
	while(it != shapes.end()) 
	{
		//bool remove = it->get()->polyShape.shouldRemoveOffScreen(ofPtr(it->get()->polyShape)
		bool remove = true;

		vector<ofPoint> &pts = it->get()->polyShape.getPoints();

		for(vector<ofPoint>::iterator itPts = pts.begin(); itPts != pts.end(); ++itPts)
			if(itPts->y <= ofGetHeight())
			{
				remove = false;
				break;
			}

		if(remove)
		{
			it->get()->polyShape.destroy();
			it = shapes.erase(it);
		}
		else
			++it;
	}

	// Add objects based on a timed interval
	currTimef = ofGetElapsedTimef();
	newObjectTimeCounter += currTimef - prevTimef;
	prevTimef = currTimef;

	if(newObjectTimeCounter > newObjectTimeMax)
	{
		int textureIndex = (int)ofRandom(textures.size());
		
		float w = textures.at(textureIndex).getWidth();
        float h = textures.at(textureIndex).getHeight();

		// TODO: Clean up magic numbers
		float randMaxX = 100.0F;
		float x = (float)(ofGetWindowWidth()/2) - (w/2.0F) + ofRandom(-randMaxX, randMaxX);
		float y = -h;
		shapes.push_back(ofPtr<TextureShape>(new TextureShape));
        shapes.back().get()->setTexture(&textures[textureIndex]);
		shapes.back().get()->setPhysics(densities.at(textureIndex), 0.25, 0.1);
        shapes.back().get()->setup(box2d, x, y, w, h);

		newObjectTimeCounter -= newObjectTimeMax;
    }

	box2d.update(); 
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(255);
	
	color.draw(viewport.x, viewport.y, viewport.width, viewport.height);

	if(drawDepth)
	{
		// draw depth
		depthImage.setFromPixels(tracker.getPixelsRef(1000, 4000));
		depthImage.draw(0, 0);
	}

	if(drawTracker)
	{
		// draw in 2D
		ofPushView();
		tracker.getOverlayCamera().begin(viewport);
		ofDrawAxis(100);
		tracker.draw();
		tracker.getOverlayCamera().end();
		ofPopView();

		ofFill();
		ofSetHexColor(0xBF2545);
		leftHandObject.draw();

		ofFill();
		ofSetHexColor(0xBF2545);
		rightHandObject.draw();
	}

	// TODO; Some management of the tracked users.
	/*int i= 0;
	while( i<tracker.getNumUser() )
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);

		if(


		i++;
	}*/

	// Draw box2d objects
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

	

	// Draw the objects
	for(list<ofPtr<TextureShape>>::iterator iter = shapes.begin(); iter != shapes.end(); ++iter)
		iter->get()->draw();

    // draw the ground
    box2d.drawGround(); 

	if(drawDebug)
	{
		string info = "";
		info += "Press [c] for circles\n";
		info += "Press [b] for blocks\n";
		info += "Press [d] for depth image\n";
		info += "Press [t] for skeleton tracker\n";
		info += "Press [x] for debug info\n";
		info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
		info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
		info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
		ofSetHexColor(0xff0000);
		ofDrawBitmapString(info, 30, 30);
	}
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
		float w = ofRandom(120, 150);
		float h = ofRandom(120, 150);
		boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
		boxes.back().get()->setPhysics(3.0, 0.25, 0.1);
		boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
    }

	if(key == 's')
	{
		float w = ofRandom(120, 150);
        float h = ofRandom(120, 150);
		shapes.push_back(ofPtr<TextureShape>(new TextureShape));
        shapes.back().get()->setTexture(&textures[(int)ofRandom(textures.size())]);
		shapes.back().get()->setPhysics(3.0, 0.25, 0.1);
        shapes.back().get()->setup(box2d, mouseX, mouseY, w, h);
    }

	if(key == 'd')
		drawDepth = !drawDepth;

	if(key == 't')
		drawTracker = !drawTracker;

	if(key == 'x')
		drawDebug = !drawDebug;
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