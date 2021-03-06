#include "ofApp.h"
bool isRand = false;
//--------------------------------------------------------------
void ofApp::setup(){
    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    left.assign(bufferSize, 0.0);
    for(int i = 0; i < objectNum; i++){
        Primitive drawObject;
        drawObject.drawMode = i % 2;
        prims.push_back(drawObject);
    }
    ofSetSphereResolution(2);
    thresh = 0.2;
    bang = false;
    bHide = false;
    panel.setup("panel");
    panel.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(300,300,300)));
    panel.add(size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)));
    panel.add(rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0)));
    panel.add(bGlobalRotate.setup("bGlobalRotate",false));
    panel.add(thresh.set("thresh", 0.25, 0.0, 1.0));
    panel.add(color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
    panel.add(bFillTog.setup("bFIll",false));
    panel.add(bReverseTog.setup("bReverseTog",false));
}
//--------------------------------------------------------------
void ofApp::update(){
    
    if(bang == false && smoothedVol > thresh){
        bang = true;
        if(isRand)randomiseAll();
        if((int)ofRandom(0,5))rotAxisMode = !rotAxisMode;
        if(bFillTog){
            bFill.x = !bFill.x;
        }
        if(bReverseTog){
            bReverse = !bReverse;
        }
        if(bGlobalRotate){
            globalRotate.set(ofRandom(0,720),
                             ofRandom(0,720),
                             ofRandom(0,720));
        }
    }
    if(bFill.x && ofGetFrameNum() % 5 == 0){
        bFill.y = !bFill.y;
    }
    if(bGlobalRotate == false){
        globalRotate.set(0,0,0);
    }
    rotate.set(ofVec3f(rotate->x,rotate->y,rotate->z));
    globalRotate.update();
    if(smoothedVol < thresh){
        bang = false;
    }
    
    
    float aVal = 1000;
    if(bFill.y == 1)aVal = 30;
    color.set(ofColor(color->r,color->g,color->b,(int)(smoothedVol * aVal)));
    ofColor currentColor = color;
    if(bReverse)currentColor.set(0,0,0,(int)(smoothedVol * aVal));
    float cVol = smoothedVol * 0.06;
    for(int i = 0; i <prims.size(); i++){
        prims[i].audioVal = smoothedVol;
        prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x + ofGetWidth() * 0.5;
        prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y + ofGetHeight() * 0.5;
        prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z;
        prims[i].rotate.x = i * cVol * rotate->x;
        prims[i].rotate.y = i * cVol * rotate->y;
        prims[i].rotate.z = i * cVol * rotate->z;
        prims[i].size.x = i * cVol * size->x + 40;
        prims[i].size.y = i * cVol * size->y + 40;
        prims[i].size.z = i * cVol * size->z + 40;
        prims[i].color = currentColor;
        prims[i].bFill = bFill.y;
        prims[i].rotAxisMode = rotAxisMode;
        prims[i].update();
    }
    
     color.set(ofColor(color->r,color->g,color->b,255));
     prims[0].audioVal = cVol;
     prims[0].pos.x = cVol * pos->x * 2.0 + ofGetWidth() * 0.5;
     prims[0].pos.y = cVol * pos->y * 2.0 + ofGetHeight() * 0.5;
     prims[0].pos.z = cVol * pos->z * 2.0;
     prims[0].rotate.x = cVol * rotate->x * 2.0;
     prims[0].rotate.y = cVol * rotate->y * 2.0;
     prims[0].rotate.z = cVol * rotate->z * 2.0;
     prims[0].size.x = cVol * size->x * 10 + 40;
     prims[0].size.y = cVol * size->y * 10 + 40;
     prims[0].size.z = cVol * size->z * 10 + 40;
     prims[0].color = color;
     prims[0].bFill = bFill.x;
     prims[0].update();
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    if(bReverse == false){
        ofBackground(0,0,0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    else {
        ofBackground(150,150,150);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateX(globalRotate.x);
    ofRotateY(globalRotate.y);
    ofRotateZ(globalRotate.z);
    ofTranslate(-ofGetWidth()*0.5, -ofGetHeight()*0.5);
    for(int i = 0; i <prims.size(); i++){
        prims[i].draw();
    }
    ofPopMatrix();
    if(bHide == false){
        panel.draw();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'a' ){
        isRand = !isRand;
    }
    if( key == 'h' ){
        bHide = !bHide;
    }
    if( key == 'z' ){
        bGlobalRotate = !bGlobalRotate;
    }
    if( key == 'x' ){
        bFillTog = !bFillTog;
    }
    if( key == 'c' ){
        bReverseTog = !bReverseTog;
    }
    if( key == ' ' ){
        ofSetWindowPosition(0, 1280);
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
void ofApp::randomiseAll(){
    int rand = (int)ofRandom(0,5);
    if(rand == 0)pos.set(ofVec3f(100,0,0));
    else if(rand == 1)pos.set(ofVec3f(0,100,0));
    else if(rand == 2)pos.set(ofVec3f(0,0,100));
    else if(rand == 3)pos.set(ofVec3f(ofRandom(50,200),ofRandom(50,200),ofRandom(50,200)));
    else if(rand == 4)pos.set(ofVec3f(0,0,0));
    rand = (int)ofRandom(0,5);
    if(rand == 0)rotate.set(ofVec3f(100,0,0));
    else if(rand == 1)rotate.set(ofVec3f(0,100,0));
    else if(rand == 2)rotate.set(ofVec3f(0,0,100));
    else if(rand == 3)rotate.set(ofVec3f(ofRandom(0,100),ofRandom(0,100),ofRandom(0,100)));
    else if(rand == 4)rotate.set(ofVec3f(0,0,0));
    rand = (int)ofRandom(0,4);
    if(rand == 0)size.set(ofVec3f(ofRandom(100,300),0,0));
    else if(rand == 1)size.set(ofVec3f(0,ofRandom(100,300),0));
    else if(rand == 2)size.set(ofVec3f(0,0,ofRandom(100,300)));
    else if(rand == 3)size.set(ofVec3f(ofRandom(0,300),ofRandom(0,300),ofRandom(0,300)));
}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    float curVol = 0.0;
    int numCounted = 0;
    for (int i = 0; i < bufferSize; i++){
        left[i]	= input[i]*0.5;
        curVol += left[i] * left[i];
        numCounted++;
    }
    curVol /= (float)numCounted;
    curVol = sqrt( curVol );
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    bufferCounter++;
}