#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main()
{
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
#ifdef _DEBUG
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
#else
	ofSetupOpenGL(&window, 1024, 768, OF_FULLSCREEN);
#endif
	ofRunApp(new testApp()); // start the app
}