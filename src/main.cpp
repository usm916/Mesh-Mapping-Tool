#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
//    window.setGlutDisplayString("rgba double samples>=4 depth");
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1280, 800, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
