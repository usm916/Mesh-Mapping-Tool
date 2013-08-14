#pragma once

#include "ofMain.h"
#include "moveObj.h"
#include "osc.h"
#include "ofxGLWarper.h"
#include "ofxXmlSettings.h"
#include "ofxSyphon.h"

class testApp : public ofBaseApp{
	public:
		void setup();
        void setupGLwarper();
		void update();
		void draw();
    
        void drawCross();
        void drawGrid(int);
	
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawHandle(ofPoint);
    
        void saveXml();
        void loadXml();
    
        int texNum = 0;
        int sTexMode = 0;
    
        ofNode nodeCam;
        float adjustMagValue = 1.0;
    
        bool bMoveObj, bWrapGrid, bEditCam, bEnableLight;
        bool bHideCursor, bRotate, bEditLookat, bUseRectangle;
        int scrW = ofGetWidth();
        int scrH = ofGetHeight();
    
//        ofEasyCam cam3d;
        vector<float* > mag;
        ofCamera cam3d;
        ofVec3f camPos, camLookat;
        MoveObjectSystem moveObjSys;
        ofLight light;
    
        ofVec2f msPress, msRelease;
        bool bDrawRectObj;
    
        ofxGLWarper warper, warper2;
        ofPoint TLPosition;
        ofPoint TRPosition;
        ofPoint BLPosition;
        ofPoint BRPosition;
    
        ofFbo scrFbo;
        ofFbo composite;
        ofVideoPlayer playMp4;
        ofTexture mySyphon;
        
        bool bDrawGrid, bDisplayInfo;
    
        ofxSyphonClient mClient;
    
        OscThread myOSC;
};
