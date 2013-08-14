#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("Model Mapper v0.01");
    myOSC.start();
    myOSC.setup();
    mag.push_back( &myOSC.mag[0] );
    mag.push_back( &myOSC.mag[1] );
    mag.push_back( &myOSC.mag[2] );
    
    mClient.setup();
//    mClient.setServerName("DEPTH");
    mClient.setServerName("F404");
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableNormalizedTexCoords();
    setupGLwarper();

    scrFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    composite.bind();
    ofClear(128);
    composite.unbind();
    moveObjSys = MoveObjectSystem(&cam3d, mClient.getSyphonAsTex(), mag);
//    moveObjSys = MoveObjectSystem(&cam3d, &ofGetCurrentViewport(), mClient[0].getSyphonAsTex(), mag);
    moveObjSys.setup();

    light.enable();
    cam3d.setFov(35.533);//Vertical
    camPos = ofVec3f(-1500.0f, -500.0f, 4150.0f);
//    camPos = ofVec3f(0.0f, 0.0f, 4150.0f);
    camLookat = ofVec3f(0.0f,0.0f,0.0f);
    cam3d.lookAt(camLookat);
    
    bEditCam = bDisplayInfo = false;
    bEditLookat = false;
    bEnableLight = true;
    bDrawRectObj = false;
    bUseRectangle = false;
    msPress = msRelease = ofVec2f(0,0);
    ofBackground(0);
    
    sTexMode = 0;
}

void testApp::setupGLwarper(){
    warper.setup(150,150,ofGetWidth()-300,ofGetHeight()-300); //initializates ofxGLWarper
    warper.deactivate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard
    warper2.setup(150,150,ofGetWidth()-300,ofGetHeight()-300); //initializates ofxGLWarper
    warper2.activate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard
    // position the image in the center of the screen
    // screen size
    float sw = ofGetWidth();
    float sh = ofGetHeight();
    // image size
    float iw = sw-300;
    float ih = sh-300;
    
    TLPosition = ofPoint(sw/2 - iw/2, sh/2 - ih/2);
    TRPosition = ofPoint(sw/2 + iw/2, sh/2 - ih/2);
    BLPosition = ofPoint(sw/2 - iw/2, sh/2 + ih/2);
    BRPosition = ofPoint(sw/2 + iw/2, sh/2 + ih/2);
    
    warper.setCorner(ofxGLWarper::TOP_LEFT, TLPosition);
    warper.setCorner(ofxGLWarper::TOP_RIGHT, TRPosition);
    warper.setCorner(ofxGLWarper::BOTTOM_LEFT, BLPosition);
    warper.setCorner(ofxGLWarper::BOTTOM_RIGHT, BRPosition);
    
    warper2.setCorner(ofxGLWarper::TOP_LEFT, TLPosition);
    warper2.setCorner(ofxGLWarper::TOP_RIGHT, TRPosition);
    warper2.setCorner(ofxGLWarper::BOTTOM_LEFT, BLPosition);
    warper2.setCorner(ofxGLWarper::BOTTOM_RIGHT, BRPosition);
}

//--------------------------------------------------------------
void testApp::update(){
//    camPos = ofVec3f(2000*sin(ofGetElapsedTimef()/3.0),2000*cos(ofGetElapsedTimef()/3.0),5000);
    cam3d.setPosition(camPos);
    cam3d.lookAt(camLookat);
    myOSC.update();
    if(!bUseRectangle)moveObjSys.update();
    
    for(int i=0;i<mag.size();i++){
        *mag[i] = *mag[i] * adjustMagValue;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    if(bHideCursor){
        ofHideCursor();
    }else{
        ofShowCursor();
    }
    ofVec3f lightPos(4*(mouseX-(ofGetWidth()/2)), 4*((ofGetHeight()/2)-mouseY), 900.0);
    
    light.setPosition(lightPos);
    light.setDiffuseColor(ofFloatColor(1.0,1.0,1.0,1.0));

    ofSetColor(0,0,0);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    glEnable(GL_DEPTH_TEST);

    if(bEnableLight){
        ofEnableLighting();
    }else{
        ofDisableLighting();
    }
    
    scrFbo.begin();
    if(!bUseRectangle){
        ofClear(0);
        cam3d.begin();
        ofPushMatrix();
        mClient.bind();
        moveObjSys.draw();
        mClient.unbind();
        ofDisableLighting();
        if(bDrawGrid)drawGrid(25);
        ofPopMatrix();
        cam3d.end();
    }else{
        ofClear(0);
        ofDisableLighting();
//        ofDisableArbTex();
        ofSetColor(255);
        mClient.bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
        glTexCoord2f(1.0, 0.0); glVertex2f(scrW, 0);
        glTexCoord2f(1.0, 1.0); glVertex2f(scrW, scrH);
        glTexCoord2f(0.0, 1.0); glVertex2f(0, scrH);
        glEnd();
        mClient.unbind();
    }
    scrFbo.end();
    
    glDisable(GL_DEPTH_TEST);
    
    ofSetColor(255);
    warper.begin();
    ofPushMatrix();
    ofScale(1,-1,1);
    scrFbo.draw(0,-scrFbo.getHeight(), scrFbo.getWidth(), scrFbo.getHeight());
    ofPopMatrix();
    //lets draw a bounding box
    if(warper.isActive()){
        int emp = 40;
        ofNoFill();
        ofSetLineWidth(1);
        ofSetHexColor(0xFF00FF);
        ofRect(1, 1, ofGetWidth()-2, ofGetHeight()-2);
        ofSetColor(200,235,255);
        drawHandle(TLPosition);
        drawHandle(TRPosition);
        drawHandle(BLPosition);
        drawHandle(BRPosition);
        ofLine( TLPosition.x+emp, TLPosition.y, TRPosition.x-emp, TRPosition.y);
        ofLine( TRPosition.x, TRPosition.y+emp, BRPosition.x ,BRPosition.y-emp);
        ofLine( BRPosition.x-emp, BRPosition.y, BLPosition.x+emp, BLPosition.y);
        ofLine( BLPosition.x, BLPosition.y-emp, TLPosition.x, TLPosition.y+emp);
    }
    warper.end();
    
    warper2.begin();
    ofPushMatrix();
    ofScale(1,-1,1);
    scrFbo.draw(0,-scrFbo.getHeight(), scrFbo.getWidth(), scrFbo.getHeight());
    ofPopMatrix();
    //lets draw a bounding box
    if(warper.isActive()){
        int emp = 40;
        ofNoFill();
        ofSetLineWidth(1);
        ofSetHexColor(0xFF00FF);
        ofRect(1, 1, ofGetWidth()-2, ofGetHeight()-2);
        ofSetColor(200,235,255);
        drawHandle(TLPosition);
        drawHandle(TRPosition);
        drawHandle(BLPosition);
        drawHandle(BRPosition);
        ofLine( TLPosition.x+emp, TLPosition.y, TRPosition.x-emp, TRPosition.y);
        ofLine( TRPosition.x, TRPosition.y+emp, BRPosition.x ,BRPosition.y-emp);
        ofLine( BRPosition.x-emp, BRPosition.y, BLPosition.x+emp, BLPosition.y);
        ofLine( BLPosition.x, BLPosition.y-emp, TLPosition.x, TLPosition.y+emp);
    }
    warper2.end();

    
    if(bDisplayInfo){
        moveObjSys.drawInfo();
        if(bDisplayInfo && !bEditCam && bDrawRectObj){
            ofEnableAlphaBlending();
            ofVec2f dlt = msRelease - msPress;
            ofNoFill();
            ofSetColor(255,255);
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofRect(msPress.x, msPress.y, dlt.x, dlt.y);
            ofFill();
            ofSetColor(255,64);
            ofRect(msPress.x, msPress.y, dlt.x, dlt.y);
        }
    }
    
    if(bEditCam){
        ofNoFill();
        ofSetColor(255,180,0);
        int size = 200;
        ofRect(ofGetWidth()/2-size/2, ofGetHeight()/2-size/2, size, size);
    }
    
    if(bDisplayInfo){
        ofRectangle curRect = ofGetCurrentViewport();
        ofDrawBitmapString("Light Position  : " + ofToString(lightPos), 50,50);
        ofDrawBitmapString("Camera Position : " + ofToString(camPos), 50,70);
        ofDrawBitmapString("Camera Look at  : " + ofToString(camLookat), 50,85);
        ofDrawBitmapString("FPS             : " + ofToString(ofGetFrameRate()), 50,100);
        ofDrawBitmapString("MAG adjust      : " + ofToString(adjustMagValue) + " - " + ofToString(moveObjSys.bModelBreak), 50, 120);
        ofDrawBitmapString("edit Mesh       : " + ofToString(moveObjSys.bMeshEdit), 50, 135);
    }
}

//--------------------------------------------------------------
void testApp::drawCross(){
    int crossSize = 30;
    ofLine(-crossSize,0,crossSize,0);
    ofLine(0,-crossSize,0,crossSize);
}

void testApp::drawHandle(ofPoint cur){
    int size = 27;
    int d = 4;
    int sht = 8;
    ofPushMatrix();
    ofTranslate(cur);
    ofLine(-size, -size, -size+size/d, -size);
    ofLine(size, -size, size-size/d, -size);
    ofLine(-size, size, -size+size/d, size);
    ofLine(size, size, size-size/d, size);
    ofLine(size, -size+size/d, size, -size);
    ofLine(size, size-size/d, size, size);
    ofLine(-size, -size+size/d, -size, -size);
    ofLine(-size, size-size/d, -size, size);
    ofLine(-size, 0, -size+sht, 0);
    ofLine(size, 0, size-sht, 0);
    ofLine(0, -size, 0, -size+sht);
    ofLine(0, size, 0, size-sht);
    ofPopMatrix();
}

void testApp::drawGrid(int num){
    for(int i=-num;i<num;i++){
        for (int j=-num;j<num; j++) {
            if(i==0 && j==0){
                ofSetColor(255,0,0);
            }else{
                ofSetColor(64);
            }
            ofPushMatrix();
            ofTranslate(i*150,j*150);
            drawCross();
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='s'){
        saveXml();
        moveObjSys.saveXml();
    }
    if(key=='l'){
        loadXml();
        moveObjSys.loadXml();
    }
    if(key=='0'){
        sTexMode = (sTexMode +1)% 2;
//        moveObjSys.setTexture(mClient[sTexMode]->getSyphonAsTex());
        cout << "syphone tex mode " << sTexMode << endl;
    }
    if(key=='L') bEnableLight = !bEnableLight;
    if(key=='{') camPos.z += 20;
    if(key=='}') camPos.z -= 20;
    if(key=='w') moveObjSys.switchWire();
    if(key=='x') moveObjSys.setMeshesNum(0);
    if(key=='r') bUseRectangle = !bUseRectangle;
//    if(key=='c') moveObjSys.setMeshesNum(1);
//    if(key=='v') moveObjSys.setMeshesNum(2);
//    if(key=='b') moveObjSys.setMeshesNum(3);
    if(key=='m') moveObjSys.bModelBreak = !moveObjSys.bModelBreak;
//    if(key=='M') moveObjSys.bMeshEdit = !moveObjSys.bMeshEdit;
    if(key=='C'){
        bEditCam = !bEditCam;
        moveObjSys.cancelSelectiveObject();
    }
    if(key=='T') bEditLookat = !bEditLookat;
    if(key=='i') {
        bDisplayInfo = !bDisplayInfo;
        if(!bDisplayInfo)moveObjSys.cancelSelectiveObject();
    }
    if(key=='e') {
         bMoveObj = !bMoveObj;
        if(warper.isActive()){
            warper.deactivate();
            warper2.activate();
        }else{
            warper.activate();
            warper2.deactivate();
        }
    }
    if(key=='_'){
        texNum -= 1;
        if(texNum<0)texNum=0;
        moveObjSys.setTexsNum( texNum );
    }
    if(key=='+'){
        texNum += 1;
        if(texNum > (moveObjSys.getTextureNum()-1) )texNum=0;
        moveObjSys.setTexsNum( texNum );
    }
    if(key=='f')ofToggleFullscreen();
    if(key=='h')bHideCursor = !bHideCursor;
    if(key=='G')bDrawGrid = !bDrawGrid;
    if(key=='<'){if(adjustMagValue>0.02)adjustMagValue -= 0.01;}
    if(key=='>'){if(adjustMagValue<10.0)adjustMagValue += 0.01;}
    if(bEditCam){
        switch (key) {
            case OF_KEY_RIGHT:
                camPos.x += 10;
                break;
            case OF_KEY_LEFT:
                camPos.x -= 10;
                break;
            case OF_KEY_UP:
                camPos.y += 10;
                break;
            case OF_KEY_DOWN:
                camPos.y -= 10;
                break;
                
            default:
                break;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(bDisplayInfo && !bEditCam){
        msRelease = ofVec2f(x,y);
        bDrawRectObj = true;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(bDisplayInfo && !bEditCam){
        moveObjSys.mouseInput();
        msPress = ofVec2f(mouseX,mouseY);
        msRelease = ofVec2f(x,y);
    }
    if(button==2)moveObjSys.cancelSelectedVertices();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(bDisplayInfo && !bEditCam){
        msRelease = ofVec2f(x,y);
        if(bDrawRectObj){
            moveObjSys.checkInsideRect(ofRectangle(msPress,msRelease));
            bDrawRectObj = false;
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//-------------------------------------------------------
void testApp::saveXml(){
    
	ofxXmlSettings saveValue;
    
	saveValue.addTag("object");
	saveValue.pushTag("object");
    
    int numSize;
    saveValue.addValue("num", numSize);
    saveValue.addTag("camPos");
    saveValue.pushTag("camPos");
    saveValue.addValue("X", camPos.x);
    saveValue.addValue("Y", camPos.y);
    saveValue.addValue("Z", camPos.z);
    saveValue.popTag();
    
    string iniValueFile = "cameraPosition.xml";
    saveValue.saveFile(iniValueFile);
    
}
//--------------------
void testApp::loadXml(){
    string iniValueFile = "cameraPosition.xml";
    string objectName;
    ofxXmlSettings loadValue;
	if(loadValue.loadFile(iniValueFile)){
        
        loadValue.pushTag("object");
        int numAtt = loadValue.getValue("num", 0.);
//        loadValue.popTag();//num
        loadValue.pushTag("camPos");
        camPos = ofVec3f(loadValue.getValue("X", 0.),loadValue.getValue("Y", 0.),loadValue.getValue("Z", 0.) );
        loadValue.popTag();//attPos
        
        cout << "num of Object :" << numAtt << endl;
        cout << "obj name : " << objectName << endl;
	}else{
		ofLog(OF_LOG_ERROR, "loadIniParam: couldn't load parametor!");
	}
}


    