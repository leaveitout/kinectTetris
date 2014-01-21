#ifndef HAND_OBJECT
#define HAND_OBJECT

#include "ofMain.h"
#include "ofxBox2dRect.h"

class HandObject :  public ofxBox2dRect
{

public:

	float calculateForces();

};

#endif