
#include "moveObj.h"

//-------   for moveObjectSystem
//-------------------------------------------------------------------

/*
 These functions are for adding quads and triangles to an ofMesh -- either
 vertices, or texture coordinates.
 */
//--------------------------------------------------------------
void addFace(ofVboMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void addFace(ofVboMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
void addTexCoords(ofVboMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

//--------------------------------------------------------------
void addTexCoords(ofVboMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d) {
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}

//--------------------------------------------------------------
void MoveObject::buildMesh(){
    // OF_PRIMITIVE_TRIANGLES means every three vertices create a triangle
    float width = 2, height = 2;//Number of Grid
	objVboMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    float gridSizeX, gridSizeY;
    
    gridSizeX = 225;
    gridSizeY = 225;
	int skip = 1;	// this controls the resolution of the mesh
	ofVec3f zero(0, 0, 0);
	for(int y = -height; y < 0; y++) {
		for(int x = 0; x < width; x++) {
            if(y==-2&&x==1){}
            else{
                ofVec3f nw = ofVec3f( x*gridSizeX, y*gridSizeY, 0);
                ofVec3f ne = ofVec3f( x*gridSizeX+gridSizeX, y*gridSizeY, 0);
                ofVec3f se = ofVec3f( x*gridSizeX+gridSizeX, y*gridSizeY+gridSizeY, 0);
                ofVec3f sw = ofVec3f( x*gridSizeX, y*gridSizeY+gridSizeY, 0);
                addFace(objVboMesh, nw, ne, se, sw);
            }
		}
	}
    setTextureCood(1.0, 1.0, 1.0);

}

void MoveObject::setTextureCood(float x, float y, float size){
    objVboMesh.clearTexCoords();
    float width = 2, height = 2;//Number of Grid
    float gridSizeX, gridSizeY;
    
    gridSizeX = 225;
    gridSizeY = 240;

	int skip = 1;	// this controls the resolution of the mesh
    for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
            if(i==1&&j==1){}
            else{
                x = ofNoise(ofGetElapsedTimef()/ofRandom(0.09,1.01),i,j);
                y = ofNoise(j,i,ofGetElapsedTimef()/ofRandom(1,1.02));
                ofVec2f nwi(-size/2+x, size/2+y);
                ofVec2f nei(size/2+x, size/2+y);
                ofVec2f swi(-size/2+x, -size/2+y);
                ofVec2f sei(size/2+x, -size/2+y);
                addTexCoords(objVboMesh, nwi, nei, sei, swi);
            }
        }
    }
}

void MoveObject::setTexNum(int num){
    texMode = num;
//    cout << "set texNum to " << texMode << endl;
}

void MoveObject::setMeshNum(int num){
    uvwMode = num;
}

//----------------------------------------
ofVec3f MoveObject::wToScr(ofVec3f WorldXYZ, ofRectangle viewport) {
    
	ofVec3f CameraXYZ = WorldXYZ * camCood->getModelViewProjectionMatrix(viewport);
	ofVec3f ScreenXYZ;
    
	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
    
	ScreenXYZ.z = CameraXYZ.z;
    
	return ScreenXYZ;
    
}

void MoveObject::initSetup(string name, ofVec3f _objScale, ofVec3f _pos, vector<ofTexture* > tex, ofCamera* cam){
    objectName = name;
    bMoveObj = false;
    bScaleObj = false;
    bRotObj = false;
    bWire = false;
    bColRnd = false;
    
    bModelBreak = false;
    bUseModelTex = false;
    bMeshEdit = false;
    bAutoSelect = true;
    
    initNearValue();
    
    pos = posOrigin = _pos;
    modelTex = tex;
    this->objScale = _objScale;
    rot = ofVec3f(0.0,0.0,0);
    for(int i=0;i<3;i++){
        life[i]=ofRandom(120,240);
        color[i]=ofColor(255);
    }
    camCood = cam;
    switchMode();
}

void MoveObject::initNearValue(){
    vector<ofVec2f > tempv2;
    vector<int > tempI;
    
    for(int j=0;j<uvwPtrnNum;j++){
        tempv2.push_back(ofVec2f(0.0,0.0));
        tempI.push_back(0);
    }
    selectedVertices.push_back(tempv2);
    selectedIndices.push_back(tempI);
    nearestDistance = 0.0;
}

void MoveObject::setup(){
//    buildMesh();
//    modelImg.loadImage("screen.png");
//    for(int i=0;i<uvwPtrnNum;i++){
//        assmpModel3ds.push_back(ofxAssimpModelLoader());
//        modelVbo.push_back(ofVboMesh());
//        assmpModel3ds[i].loadModel("mesh0606_CC.3DS", true);
//        assmpModel3ds[i].enableNormals();
//        vector<string> names = assmpModel3ds[i].getMeshNames();
//        cout << "numMesh " << assmpModel3ds[i].getNumMeshes() << endl;
//        cout << "posMesh " << assmpModel3ds[i].getPosition() << endl;
//
//        for(int j=0;j<names.size();j++){
//            cout << "name " << j << " : " << names[j] << endl;
//        }
//        modelVbo[i] = assmpModel3ds[i].getMesh(0);
//    }
//    
//    ofAddListener(ofEvents().update, this, &MoveObject::updatePos);
//    ofQuaternion xRot, yRot, zRot;
//    xRot = ofQuaternion(rot.x, ofVec3f(1,0,0));
//    yRot = ofQuaternion(rot.y, ofVec3f(0,1,0));
//    zRot = ofQuaternion(rot.z, ofVec3f(0,0,1));
//    
//    curRot *= yRot * xRot * zRot;
}

void MoveObject::setup(string _name, ofVec3f _pos){
    pos = _pos;
    
    buildMesh();
    modelImg.loadImage("screen.png");
    for(int i=0;i<uvwPtrnNum;i++){
        assmpModel3ds.push_back(ofxAssimpModelLoader());
        modelVbo.push_back(ofVboMesh());
        modelAdjustedVbo.push_back(ofVboMesh());
        assmpModel3ds[i].loadModel(_name + ofToString(i) + ".3DS", true);
        vector<string> names = assmpModel3ds[i].getMeshNames();
        cout << "numMesh " << assmpModel3ds[i].getNumMeshes() << endl;
        cout << "posMesh " << assmpModel3ds[i].getPosition() << endl;
        for(int j=0;j<names.size();j++){
            cout << "name " << j << " : " << names[j] << endl;
        }
        modelVbo[i] = assmpModel3ds[i].getMesh(0);
        modelAdjustedVbo[i] = modelVbo[i];
        for(int j=0;j<modelAdjustedVbo[i].getNumVertices();j++){
            modelAdjustedVbo[i].setVertex(j, ofVec3f(0,0,0));
        }
        modelAdjustedVbo[i].load(_name + ofToString(i) + ".ofm");
    }
    
    ofAddListener(ofEvents().update, this, &MoveObject::updatePos);
    ofQuaternion xRot, yRot, zRot;
    xRot = ofQuaternion(rot.x, ofVec3f(1,0,0));
    yRot = ofQuaternion(rot.y, ofVec3f(0,1,0));
    zRot = ofQuaternion(rot.z, ofVec3f(0,0,1));
    
    curRot *= yRot * xRot * zRot;
    
    /////----------------------------------------------------------------------------------------warper init
    vector<ofVec3f > tmp = modelVbo[0].getVertices();
    float minX, minY, maxX, maxY, minZ, maxZ;
    minX = minY = maxX = maxY = minZ = maxZ = 0.0;
    for(int i=0;i<tmp.size();i++){
        if(i==0){
            minX = maxX = tmp[i].x;
            minY = maxY = tmp[i].y;
            minZ = maxZ = tmp[i].z;
        }else{
            if(minX > tmp[i].x)minX = tmp[i].x;
            if(maxX < tmp[i].x)minX = tmp[i].x;
            if(minY > tmp[i].y)minX = tmp[i].y;
            if(maxY < tmp[i].y)minX = tmp[i].y;
            if(minZ > tmp[i].z)minX = tmp[i].z;
            if(maxZ < tmp[i].z)minX = tmp[i].z;
        }
    }
    warper.setup(minX,minY,maxX,maxY); //initializates ofxGLWarper
    warper.deactivate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard
    // position the image in the center of the screen
    // screen size
    float sw = maxX;//ofGetWidth();
    float sh = maxY;//ofGetHeight();
    // image size
    float offset = 0;
    float iw = sw-offset;
    float ih = sh-offset;
    
    TLPosition = ofPoint(minX,maxY);//sw/2 - iw/2, sh/2 - ih/2);
    TRPosition = ofPoint(maxX,maxY);//sw/2 + iw/2, sh/2 - ih/2);
    BLPosition = ofPoint(minX,minY);//ofPoint(sw/2 - iw/2, sh/2 + ih/2);
    BRPosition = ofPoint(maxX,minY);//sw/2 + iw/2, sh/2 + ih/2);
    
    warper.setCorner(ofxGLWarper::TOP_LEFT, TLPosition);
    warper.setCorner(ofxGLWarper::TOP_RIGHT, TRPosition);
    warper.setCorner(ofxGLWarper::BOTTOM_LEFT, BLPosition);
    warper.setCorner(ofxGLWarper::BOTTOM_RIGHT, BRPosition);
}

void MoveObject::applyAdjustValue(){
    for(int i=0;i<modelAdjustedVbo.size();i++){
        for(int j=0;j<modelAdjustedVbo[i].getNumVertices();j++){
            ofVec3f cur = modelVbo[i].getVertex(j);
            ofVec3f adjust = modelAdjustedVbo[i].getVertex(j);
            modelVbo[i].setVertex(j, cur + adjust);
        }
    }
}

void MoveObject::update(){
    pmx = mx;
    pmy = my;
    mx = ofGetMouseX();
    my = ofGetMouseY();
    drawMesh.clear();
    drawMesh.push_back( modelVbo[uvwMode] );
}

void MoveObject::updateScreenVertices(){
    if( selectedIndices[0].size()!=0 ){
        for(int i=0;i<selectedIndices[0].size();i++){
            selectedVertices[0][i] = camCood->worldToScreen( (objScale * (curRot * modelVbo[0].getVertex(selectedIndices[0][i]))) + pos );
        }
    }
}

void MoveObject::meshDistortion(){
    
    int numVtx = drawMesh[0].getNumVertices();
    float ms = (float)ofGetMouseX()/(float)ofGetWidth();
    float adjust = 10;
    if(*mag[1]>0.1){
        for(int i=0;i<numVtx;i++){
            ofVec3f cur = drawMesh[0].getVertex(i);
            drawMesh[0].setVertex(i, ofVec3f(cur.x + *mag[i%3] * (ofNoise(ofGetElapsedTimef(),cur.x)-.5),
                                             cur.y + *mag[i%3] * (ofNoise(ofGetElapsedTimef(),cur.y)-.5),
                                             cur.z+adjust * *mag[i%3] *(ofNoise(ofGetElapsedTimef()* *mag[1] * 20,cur.x,cur.y,cur.z)-0.5)));
        }
    }
}

//CAUTION YOU NEED TO PUT THIS FUNCTION AFTER meshDestortion()
void MoveObject::meshDeleter(int numRemain){
    //
}

//--
void MoveObject::updatePos(ofEventArgs& args){
    if(bMoveObj){
        float mm = 0.2;
        if(ofGetKeyPressed(OF_KEY_UP))pos.y += mm;
        if(ofGetKeyPressed(OF_KEY_DOWN) )pos.y -= mm;
        if(ofGetKeyPressed(OF_KEY_RIGHT) )pos.x += mm;
        if(ofGetKeyPressed(OF_KEY_LEFT) )pos.x -= mm;
        if(ofGetKeyPressed('.'))pos.z += mm;
        if(ofGetKeyPressed(','))pos.z -= mm;
    }
    if(bMoveObjL){
        float mm = 3.0;
        if(ofGetKeyPressed(OF_KEY_UP))pos.y += mm;
        if(ofGetKeyPressed(OF_KEY_DOWN) )pos.y -= mm;
        if(ofGetKeyPressed(OF_KEY_RIGHT) )pos.x += mm;
        if(ofGetKeyPressed(OF_KEY_LEFT) )pos.x -= mm;
        if(ofGetKeyPressed('.'))pos.z += mm;
        if(ofGetKeyPressed(','))pos.z -= mm;
    }
    if(bRotObj){
        ofQuaternion xRot, yRot, zRot;
        float mm = 0.03;
        if(ofGetKeyPressed( OF_KEY_UP ))   { xRot = ofQuaternion(mm, ofVec3f(1,0,0)); }
        if(ofGetKeyPressed( OF_KEY_DOWN )) { xRot = ofQuaternion(-mm, ofVec3f(1,0,0)); }
        if(ofGetKeyPressed( OF_KEY_RIGHT )){ yRot = ofQuaternion(mm, ofVec3f(0,1,0)); }
        if(ofGetKeyPressed( OF_KEY_LEFT )) { yRot = ofQuaternion(-mm, ofVec3f(0,1,0)); }
        if(ofGetKeyPressed( '.' ))         { zRot = ofQuaternion(-mm, ofVec3f(0,0,1)); }
        if(ofGetKeyPressed( ',' ))         { zRot = ofQuaternion(mm, ofVec3f(0,0,1)); }
        curRot *= yRot * xRot * zRot;
    }
    if(bScaleObj){
        float mm = 0.001;
        if(ofGetKeyPressed(OF_KEY_UP))objScale.y += mm;
        if(ofGetKeyPressed(OF_KEY_DOWN) )objScale.y -= mm;
        if(ofGetKeyPressed(OF_KEY_RIGHT) )objScale.x += mm;
        if(ofGetKeyPressed(OF_KEY_LEFT) )objScale.x -= mm;
        if(ofGetKeyPressed('.'))objScale.z += mm;
        if(ofGetKeyPressed(','))objScale.z -= mm;
    }
    if(bMeshEdit){
        float mm = 0.2;
        for(int j=0;j<uvwPtrnNum;j++){
            for(int i=0;i<selectedIndices[j].size();i++){
                ofVec3f cur = modelVbo[j].getVertex(selectedIndices[j][i]);
                ofVec3f adjVtx = modelAdjustedVbo[j].getVertex(selectedIndices[j][i]);
                ofVec3f tmp(0,0,0);
                if(ofGetKeyPressed(OF_KEY_UP)) tmp.y += mm;
                if(ofGetKeyPressed(OF_KEY_DOWN) ) tmp.y -= mm;
                if(ofGetKeyPressed(OF_KEY_RIGHT) ) tmp.x += mm;
                if(ofGetKeyPressed(OF_KEY_LEFT) ) tmp.x -= mm;
                if(ofGetKeyPressed('.')) tmp.z += mm;
                if(ofGetKeyPressed(',')) tmp.z -= mm;
                modelVbo[j].setVertex(selectedIndices[j][i], cur + tmp);
                modelAdjustedVbo[j].setVertex(selectedIndices[j][i], adjVtx + tmp);
            }
        }
    }
}

//--
void MoveObject::draw(){
    if(ofGetKeyPressed('j')){
        setTextureCood(ofNoise(ofGetElapsedTimef()*1), ofNoise(ofGetElapsedTimef()*0.9,6), ofNoise(ofGetElapsedTimef()*2, 12));
    }else if(ofGetKeyPressed('k')){
        setTextureCood(0.5, 0.5, 1.0);
    }else if(ofGetKeyPressed('l')){
        setTextureCood(ofNoise(ofGetElapsedTimef()*2), ofNoise(ofGetElapsedTimef()*4,6), ofNoise(ofGetElapsedTimef()*3, 12));
    }
    if(bMoveObj||bRotObj){
        ofSetColor(255, 255, 0);
    }else{
        ofSetColor(255);
    }
//    warper.begin();
    ofPushMatrix();
    glEnable(GL_DEPTH_TEST);
    ofTranslate(pos);
    
    ofVec3f axis;
    float angle;
    curRot.getRotate(angle, axis);
	ofRotate(angle, axis.x, axis.y, axis.z);
    
    ofScale(objScale.x, objScale.y, objScale.z);
    ofFill();
    if(bDrawObj)drawObj();
    if(bMoveObj||bRotObj){
        glDisable(GL_DEPTH_TEST);
        if(bMoveObj)ofSetColor(255,0,0);
        if(bRotObj)ofSetColor(0,190,50);
        ofBox(0,0,objScale.x*60);
    }
    ofPopMatrix();
    ofLine( TLPosition,TRPosition );
    ofLine( BLPosition,BRPosition );

    //    warper.end();

    cnt++;
}

void MoveObject::drawObj(){
    // mm
    int unit = 225;
    int adv = ofRandom(1,10);
    int lineWidth;
    if(bEdit)lineWidth = 1;
    else lineWidth = 3;
//    if(ofGetFrameNum()%60 == 0) meshMode = (int)ofRandom(0,modelUvPtrn-0.05);
    
    if(cnt%life[0]==0 && bColRnd){
        color[0] = randomColor();
    }else if(!bColRnd){
        color[0] = ofColor(255);
    }
    ofSetColor(color[0]);
    ofPushMatrix();
    
    modelTex[texMode]->bind();
    if(!bWire){
        drawMesh[0].draw();
    }else{
        ofSetLineWidth(3);
        drawMesh[0].drawWireframe();
    }
    modelTex[texMode]->unbind();
    
    ofPopMatrix();
}

ofVec3f slv(1,1,0);

void MoveObject::drawInfo(int num){
    stringstream ss;
    ss.str("");
    ss << num+1 << "   " << modeName << "\n\n\n" << objectName << ".3DS\nPosition : " << (pos-posOrigin) << "\nRotation : " << curRot.getEuler() << "\nScale : " << objScale;
    ofPushMatrix();
    ofVec2f posInfo = camCood->worldToScreen(pos);
    ofTranslate(posInfo);
    ofSetColor(geomModeColor[geomModeCnt%6]);
    ofSetLineWidth(1);
    ofDrawBitmapString(ss.str(), -5, 3);
    int size = 20;
    int d = 3;
    int sht = 6;
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

//--
bool MoveObject::checkPos(int x, int y, float radius){
    ofVec3f posScr;
    float dist;
    posScr = camCood->worldToScreen(pos);
    dist = posScr.distance(ofVec3f(x,y,0));
    if(radius>dist){
        return true;
    }else{
        return false;
    }
}

//--
void MoveObject::checkRect(ofRectangle _rect){
    selectedVertices.clear();
    selectedIndices.clear();
    
    ofVec2f mouse(mx,my);
    ofVec3f posScr;
    for(int j=0;j<uvwPtrnNum;j++){
        int n = modelVbo[j].getNumVertices();
        ofVec2f pcur;
        int pi;
        vector<ofVec2f> neaV;
        vector<int> neaI;
        for(int i = 0; i < n; i++) {
            ofVec2f cur = camCood->worldToScreen( (objScale * (curRot * modelVbo[j].getVertex(i))) + pos );
            if( _rect.inside(cur) ) {
                neaV.push_back(cur);
                neaI.push_back(i);
            }
        }
        selectedVertices.push_back(neaV);
        selectedIndices.push_back(neaI);
    }
}

void MoveObject::calNearMeshCur(int x, int y){
    if(mx!=pmx && my!=pmy){
//        cout << "calc" << endl;
        selectedVertices.clear();
        selectedIndices.clear();
        
        ofVec2f mouse(mx,my);
        ofVec3f posScr;
        for(int j=0;j<uvwPtrnNum;j++){
            int n = modelVbo[j].getNumVertices();
            ofVec2f pcur;
            int pi;
            vector<ofVec2f> neaV;
            vector<int> neaI;
            for(int i = 0; i < n; i++) {
                ofVec2f cur = camCood->worldToScreen( (objScale * (curRot * modelVbo[j].getVertex(i))) + pos );
                float distance = cur.distance(mouse);
                if( i == 0 || distance < nearestDistance ) {
                    nearestDistance = distance;
                    pcur = cur;
                    pi = i;
                }
            }
            for(int i = 0; i < n; i++) {
                ofVec2f cur = camCood->worldToScreen( (objScale * (curRot * modelVbo[j].getVertex(i))) + pos );
                float distance = cur.distance(pcur);
                if( distance < 1.0){
                    neaV.push_back(cur);
                    neaI.push_back(i);
                }
            }
            selectedVertices.push_back(neaV);
            selectedIndices.push_back(neaI);
        }
    }
}

void MoveObject::drawPickedCursor(){
	ofSetColor(ofColor::gray);
//	ofSetLineWidth(1.0);
    if( selectedIndices[0].size() != 0 && bAutoSelect){
        ofLine(selectedVertices[0][0], ofVec2f(ofGetMouseX(),ofGetMouseY()));
    
        ofSetColor(geomModeColor[5]);
        ofSetLineWidth(2.0);
        ofPushMatrix();
        ofSetLineWidth(1.0);
        ofTranslate(selectedVertices[0][0]);
        ofRotateZ(ofGetElapsedTimef()*40);
        ofNoFill();
        ofCircle( 0.0f, 0.0f, 8.0f);
        int n = drawMesh[uvwMode].getNumVertices();

        float cirResolution = 180;
        float per = (float)selectedIndices[0][0]/(float)n;
        float res = 360.0f*per;
        float cirSize = 40.0f;
        float cirSize2 = 30.0f;
        float step = 12.0;
        float s = 70;
        for(float i=0;i<res;i+=step){
            ofLine(cirSize * sin(i/s), cirSize * cos(i/s), cirSize * sin((i+step)/s), cirSize * cos((i+step)/s));
            ofLine(cirSize2 * sin(-i/s), cirSize2 * cos(-i/s), cirSize2 * sin(-(i+step)/s), cirSize2 * cos(-(i+step)/s));
        }
        ofPopMatrix();
        ofSetLineWidth(1.0);
    }else if( selectedIndices[0].size() != 0 && !bAutoSelect ){
        ofSetColor(geomModeColor[5]);
        for(int i=0;i<selectedIndices[0].size();i++){
            for(int j=0;j<uvwPtrnNum;j++){
                ofNoFill();
                ofSetLineWidth(2);
                ofCircle(selectedVertices[j][i], 6);
            }
        }

    }

}

void MoveObject::setMode(int num){
    geomModeCnt = num-1;
    switchMode();
}

void MoveObject::switchMode(){
    geomModeCnt = (geomModeCnt + 1)%6;
    if(geomModeCnt==0){
        modeName = "Mode : ----";
        bMoveObj = false;
        bMoveObjL = false;
        bRotObj = false;
        bScaleObj = false;
        bMeshEdit = false;
        initNearValue();
    }
    if(geomModeCnt==1){
        modeName = "Mode : Adjust mesh Position";
        bMoveObj = true;
        bMoveObjL = false;
        bRotObj = false;
        bScaleObj = false;
        bMeshEdit = false;
        initNearValue();
    }
    if(geomModeCnt==2){
        modeName = "Mode : Move mesh Position";
        bMoveObj = false;
        bMoveObjL = true;
        bRotObj = false;
        bScaleObj = false;
        bMeshEdit = false;
        initNearValue();
    }
    if(geomModeCnt==3){
        modeName = "Mode : Rotate mesh";
        bMoveObj = false;
        bMoveObjL = false;
        bRotObj = true;
        bScaleObj = false;
        bMeshEdit = false;
        initNearValue();
    }
    if(geomModeCnt==4){
        modeName = "Mode : Scale mesh";
        bMoveObj = false;
        bMoveObjL = false;
        bRotObj = false;
        bScaleObj = true;
        bMeshEdit = false;
        initNearValue();
    }
    if(geomModeCnt==5){
        modeName = "Mode : Adjust mesh";
        bMoveObj = false;
        bMoveObjL = false;
        bRotObj = false;
        bScaleObj = false;
        bMeshEdit = true;
        initNearValue();
    }
}

//-------   for MoveObjectSystem
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MoveObjectSystem::setup(){

    meshImg.push_back( ofImage("flower.jpg") );
    meshImg.push_back( ofImage("flowerC.jpg") );
    meshImg.push_back( ofImage("screen.png") );
    meshTex.push_back( mySyphon );
    meshTex.push_back( &meshImg[0].getTextureReference() );
    meshTex.push_back( &meshImg[1].getTextureReference() );
    meshTex.push_back( &meshImg[2].getTextureReference() );
    meshTex[0]->texData.bFlipTexture = true;
    meshTex[1]->texData.bFlipTexture = false;
    meshTex[2]->texData.bFlipTexture = true;

    
    
    ofAddListener(ofEvents().update, this, &MoveObjectSystem::mousePressed);
    bMoveObj = bDrawObjByMouse = bModelBreak = bMeshEdit = false;
    float scale = 200.0;
    loadShader();//ssao
    int numObj = 5;
    
    ofVec3f scl = ofVec3f(1.0f, 1.0f, 1.0f);
    myObj.push_back( new MoveObject(ofVec3f(-1890.566528,773.386902,457.177094), scl, "fllmesh_L_", meshTex, cam3d, mag) );
    myObj.push_back( new MoveObject(ofVec3f(-1717.561157,-203.361145,593.306030), scl, "fllmesh_S_", meshTex, cam3d, mag) );
    myObj.push_back( new MoveObject(ofVec3f(0.000000,0.000000,0.000000), scl, "fllmesh_C_", meshTex, cam3d, mag) );
    myObj.push_back( new MoveObject(ofVec3f(1875.159180,293.244354,458.060059), scl, "fllmesh_SC_", meshTex, cam3d, mag) );
    myObj.push_back( new MoveObject(ofVec3f(1736.474243,-827.870850,592.028809), scl, "mesh0606_RD_", meshTex, cam3d, mag) );
    
}

//--
void MoveObjectSystem::update(){
    for(int i=0;i<myObj.size();i++){
        myObj[i]->update();
        if(bModelBreak)myObj[i]->meshDistortion();
        if(myObj[i]->bMeshEdit){
            myObj[i]->updateScreenVertices();
            if(myObj[i]->bAutoSelect)myObj[i]->calNearMeshCur(ofGetMouseX(),ofGetMouseY());
        }
    }
}

//--
void MoveObjectSystem::checkInsideRect(ofRectangle _rect){
    for(int i=0;i<myObj.size();i++){
        if(myObj[i]->bMeshEdit){
            myObj[i]->bAutoSelect = false;
            myObj[i]->checkRect(_rect);
        }
    }
}

//--
void MoveObjectSystem::draw(){
    if(!myObj.empty()){
        if(bDrawObjByMouse){
            drawMouseCheckObj();
        }else{
            drawAllObj();
        }
        for(int i=0;i<myObj.size();i++){
            myObj[i]->draw();
        }
    }
}

void MoveObjectSystem::drawInfo(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->drawInfo(i);
            //JUST TEST -> PICKED CURSOR
            if(myObj[i]->bMeshEdit)myObj[i]->drawPickedCursor();
        }
    }
}

//---
//-------------------------------------------------------
void MoveObjectSystem::saveXml(){
    
	ofxXmlSettings saveValue;
    
	saveValue.addTag("object");
	saveValue.pushTag("object");
    
    int numSize;
    if(!myObj.empty()){
        numSize = myObj.size();
    }else{
        numSize = 0;
    }
    saveValue.addValue("num", numSize);
    
    saveValue.addTag("objPos");
    if(!myObj.empty()){
        int i=0;
        for(int i=0;i<myObj.size();i++){
            saveValue.pushTag("objPos");
            saveValue.addValue("name"+ofToString(i), myObj[i]->objectName);
            saveValue.addValue("scaleX"+ofToString(i), myObj[i]->objScale.x);
            saveValue.addValue("scaleY"+ofToString(i), myObj[i]->objScale.y);
            saveValue.addValue("scaleZ"+ofToString(i), myObj[i]->objScale.z);
            saveValue.addValue("X"+ofToString(i), myObj[i]->pos.x);
            saveValue.addValue("Y"+ofToString(i), myObj[i]->pos.y);
            saveValue.addValue("Z"+ofToString(i), myObj[i]->pos.z);
            for(int j=0;j<4;j++){
                ofMatrix4x4 quad;
                myObj[i]->curRot.get(quad);
                saveValue.addValue("rotx"+ofToString(i)+ofToString(j), quad._mat[j].x);
                saveValue.addValue("roty"+ofToString(i)+ofToString(j), quad._mat[j].y);
                saveValue.addValue("rotz"+ofToString(i)+ofToString(j), quad._mat[j].z);
                saveValue.addValue("rotw"+ofToString(i)+ofToString(j), quad._mat[j].w);
            }
            saveValue.popTag();
            for(int j=0;j<myObj[i]->modelAdjustedVbo.size();j++){
                myObj[i]->modelAdjustedVbo[j].save(myObj[i]->objectName + ofToString(j) + ".ofm");
            }
        }
    }
    
    string iniValueFile = "objectPosition.xml";
    saveValue.saveFile(iniValueFile);
    
}
//--------------------
void MoveObjectSystem::loadXml(){
    string iniValueFile = "objectPosition.xml";
    string objectName;
    ofxXmlSettings loadValue;
	if(loadValue.loadFile(iniValueFile)){
        
        myObj.clear();
        
        loadValue.pushTag("object");
        int numAtt = loadValue.getValue("num", 0.);
        loadValue.pushTag("objPos");
        for(int i=0;i<numAtt;i++) {
            string objectName = loadValue.getValue("name"+ofToString(i), "none");
            ofVec3f _objScale = ofVec3f(loadValue.getValue("scaleX"+ofToString(i), 1.0),
                                        loadValue.getValue("scaleY"+ofToString(i), 1.0),
                                        loadValue.getValue("scaleZ"+ofToString(i), 1.0));
            ofVec3f _pos = ofVec3f(loadValue.getValue("X"+ofToString(i), 0.),
                                   loadValue.getValue("Y"+ofToString(i), 0.),
                                   loadValue.getValue("Z"+ofToString(i), 0.) );
            cout << i << ", " << _pos << endl;

            ofMatrix4x4 q;
            for(int j=0;j<4;j++){
                q._mat[j].x = loadValue.getValue("rotx"+ofToString(i)+ofToString(j), 0.0);
                q._mat[j].y = loadValue.getValue("roty"+ofToString(i)+ofToString(j), 0.0);
                q._mat[j].z = loadValue.getValue("rotz"+ofToString(i)+ofToString(j), 0.0);
                q._mat[j].w = loadValue.getValue("rotw"+ofToString(i)+ofToString(j), 0.0);
            }
            myObj.push_back( new MoveObject(_pos, _objScale, objectName, q, meshTex, cam3d, mag) );
            myObj[i]->applyAdjustValue();
        }
        loadValue.popTag();//attPos
        

        cout << "num of Object :" << numAtt << endl;
        cout << "obj name : " << objectName << endl;
	}else{
		ofLog(OF_LOG_ERROR, "loadIniParam: couldn't load parametor!");
	}
}

void MoveObjectSystem::switchWire(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->bWire = !myObj[i]->bWire;
        }
    }
}

void MoveObjectSystem::switchColorRandom(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->bColRnd = !myObj[i]->bColRnd;
        }
    }
}

void MoveObjectSystem::loadShader(){
//    ssao.setup( ofGetWidth(), ofGetHeight(), GL_RGBA );
}

void MoveObjectSystem::drawMouseCheckObj(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            if(myObj[i]->checkPos(ofGetMouseX(), ofGetMouseY(), 100)){
                myObj[i]->bDrawObj = true;
            }else{
                myObj[i]->bDrawObj = false;
            }
        }
    }
}

void MoveObjectSystem::drawAllObj(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->bDrawObj = true;
        }
    }
}

int MoveObjectSystem::getTextureNum(){
    return meshTex.size();
}

void MoveObjectSystem::setTexsNum(int num){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->setTexNum(num);
        }
    }
}

void MoveObjectSystem::setMeshesNum(int num){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->setMeshNum(num);
        }
    }
}

//--
void MoveObjectSystem::mouseInput(){
    float searchRadius =25.0;
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            if(myObj[i]->checkPos(ofGetMouseX(), ofGetMouseY(), searchRadius))myObj[i]->switchMode();
        }
    }
}
void MoveObjectSystem::cancelSelectiveObject(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->setMode(0);
        }
    }
}
void MoveObjectSystem::cancelSelectedVertices(){
    if(!myObj.empty()){
        for(int i=0;i<myObj.size();i++){
            myObj[i]->bAutoSelect = true;
        }
    }
}

void MoveObjectSystem::mousePressed(ofEventArgs& args){
//    cout << "mouseButton" << ofGetMousePressed() << endl;
//    if(ofGetMousePressed(0)){
//        for(int i=0;i<myObj.size();i++){
//            if(myObj[i]->checkPos(ofGetMouseX(), ofGetMouseY(), searchRadius))myObj[i]->switchMode(aobjedit);
//            if(myObj[i]->bRotObj) myObj[i]->bRotObj = false;
//            if(myObj[i]->bRotObj) myObj[i]->bRotObj = false;
//            if(myObj[i]->bRotObj) myObj[i]->bRotObj = false;
//        }
//    }else if(ofGetMousePressed(2)){
//        for(int i=0;i<myObj.size();i++){
//            if(myObj[i]->checkPos(ofGetMouseX(), ofGetMouseY(), searchRadius))myObj[i]->switchMode(arotate);
//            if(myObj[i]->bMoveObj) myObj[i]->bMoveObj = false;
//        }
//    }
}
