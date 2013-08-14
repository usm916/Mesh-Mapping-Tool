//
//  moveObj.h
//  emptyExample
//
//  Created by usm on 5/22/13.
//
//

#ifndef emptyExample_moveObj_h
#define emptyExample_moveObj_h

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGLWarper.h"
//#include "ofxSSAO.h"

#define TexNUM 10

enum adjustMode{
    amove,
    ascale,
    arotate,
    amodelBreak,
};

class MoveObject {
public:
    
    MoveObject(){
    }
    MoveObject(ofVec3f _pos, ofVec3f _objScale, string name, vector<ofTexture* > tex, ofCamera* cam, vector<float* > _mag){
        initSetup(name, _objScale, _pos, tex, cam);
        mag = _mag;
        setup(name, _pos);
    }
    MoveObject(ofVec3f _pos, ofVec3f _objScale, string name, ofMatrix4x4 quad, vector<ofTexture* > tex, ofCamera* cam, vector<float* > _mag){
        initSetup(name, _objScale, _pos, tex, cam);
        mag = _mag;
        curRot.set(quad);
        setup(name, _pos);
    }

    string modeName = "";
    const int uvwPtrnNum = 1;
    int uvwMode = 0;
    int textureMode = 0;
    int geomModeCnt = -1;
//    int *pmx, *pmy, *mx, *my;
    int pmx, pmy, mx, my;
    
    ofColor color[3];
    ofVec3f pos, rot, posOrigin, objScale;
    int cnt, life[3];
    string objectName;
    bool bMoveObj, bMoveObjL, bScaleObj, bRotObj, bMeshEdit, bUseModelTex;
    bool bScrInfo, bEdit, bWire, bColRnd, bDrawObj, bModelBreak, bAutoSelect;
    
    ofImage modelImg;
    
    vector<ofxAssimpModelLoader> assmpModel3ds;//from Assmp Loader.
    vector<ofTexture* > modelTex;//when render model then use the tex pointer.
    vector<ofVboMesh> modelVbo;//cache Original form of model.
    vector<ofVboMesh> modelAdjustedVbo;//Effected model(e.g. noise or wave etc). when you render use this model.
//    vector<ofVec4f > adjustVertex;
    ofVboMesh objVboMesh;
    vector<ofVboMesh> drawMesh;//ofVboMesh's clear() looks like does not work.
    int texMode = 0;
    
    ofQuaternion curRot;
    
    void initSetup(string name, ofVec3f _objScale, ofVec3f _pos, vector<ofTexture* > tex, ofCamera* cam);
    void setup();
    void setup(string, ofVec3f);
    void update();
    void updateScreenVertices();
    
    void draw();
    inline void drawObj();
    void drawInfo(int);
    void updatePos(ofEventArgs& args);
    void meshDistortion();
    void meshDeleter(int);
    void drawCenter();
    void buildMesh();
    void setTextureCood(float x, float y, float size);
    void setTexNum(int);
    void setMeshNum(int);
    void drawPickedCursor();
    void initNearValue();
    void applyAdjustValue();
    //change Edit Object Setting
    void switchMode();
    void setMode(int);

    ///////// EDIT VERTEX
    bool checkPos(int x, int y, float radius);
    inline void checkRect(ofRectangle _croped);
    inline void calNearMeshCur(int, int);
    ofVec2f mouse;

    //Need to prepare vector vector because Mesh UVW pattern = 4;
    vector<vector<ofVec2f> > selectedVertices;
	vector<vector<int> > selectedIndices;
	float nearestDistance;

    vector<float* > mag;
    
    ofCamera* camCood;
    ofVec3f wToScr(ofVec3f,ofRectangle);
    /////////warper set
    ofxGLWarper warper;
    ofPoint TLPosition;
    ofPoint TRPosition;
    ofPoint BLPosition;
    ofPoint BRPosition;
    
    ofColor randomColor(){
        ofColor color(ofRandom(150,255),ofRandom(120,255),ofRandom(40,130));
        return color;
    }
    ofColor geomModeColor[6] = { ofColor(255,255,196), ofColor(255,64,64), ofColor(255,128,96), ofColor(128,255,0), ofColor(0,128,255), ofColor(196,128,255) };
    
};

// MoveObjectSystem ------------------------------------------------------------------------------------------------------------------------------------//
class MoveObjectSystem {
    public:
        MoveObjectSystem(){}
        MoveObjectSystem(ofCamera* cam, ofTexture* syp, vector<float *> _mag){
            mag = _mag;
            cam3d = cam;
            mySyphon = syp;
        }
    
        void setup();
        void loadShader();
        void update();
        void draw();
        void drawInfo();
        void drawAllObj();
        void drawMouseCheckObj();
    
    
        void setTexture(ofTexture* tx){
            mySyphon = tx;
        }
    
        void switchWire();
        void switchColorRandom();
    
        void saveXml();
        void loadXml();

        void mouseInput();
        void cancelSelectiveObject();
        void cancelSelectedVertices();
        void mousePressed(ofEventArgs& args);
        void checkInsideRect(ofRectangle);
    
        bool bMoveObj, bDrawObjByMouse, bModelBreak, bMeshEdit;
    
        int getTextureNum();
        void setTexsNum(int);
        void setMeshesNum(int);
        ofCamera* cam3d;
        vector<MoveObject *> myObj;
        vector<float* > mag;
    
        ofTexture* mySyphon;
        int numImg;
        vector<ofImage> meshImg;
        vector<ofTexture* > meshTex;
};
#endif
