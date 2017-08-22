#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    
    testFont.loadFont("GenShinGothic-Monospace-Regular.ttf", 64, false, true);
    
    sources.push_back(str);
    sources.push_back(str2);
    sources.push_back(str3);

    paths = testFont.getStringAsPoints(str, 0, 64, 600);
    paths2 = testFont.getStringAsPoints(str2, 0, 0, 600);
    
    for (int i = 0; i < 40000; i++){
        particle myParticle;
        myParticle.setInitialCondition
        (ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
        particles.push_back(myParticle);
    }
    VF.setupField(paths, 600, 400, ofGetWidth(), ofGetHeight());
    addMode = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (addMode == 0) {
        return;
    }
    for (int i = 0; i < particles.size(); i++){
        
        //particleの力をリセット
        particles[i].resetForce();
        
        //ベクトル場から、それぞれのparticleにかかる力を算出
        ofVec2f frc;
        frc = VF.getForceFromPos(particles[i].pos.x, particles[i].pos.y);
        
        //Particleの状態を更新
        particles[i].addForce(frc.x, frc.y);
        particles[i].addDampingForce();
        particles[i].bounceOffWalls();
        particles[i].update();
    }
    
    //ベクトル場の力の減衰
    //VF.fadeField(0.998f);
    for (int i = 0; i < particles.size(); i++){
        
        //particleの力をリセット
        particles[i].resetForce();
        
        //ベクトル場から、それぞれのparticleにかかる力を算出
        ofVec2f frc;
        frc = VF.getForceFromPos(particles[i].pos.x, particles[i].pos.y);
        
        //Particleの状態を更新
        particles[i].addForce(frc.x, frc.y);
        particles[i].addDampingForce();
        particles[i].bounceOffWalls();
        particles[i].update();
    }
    
    //ベクトル場の力の減衰
//    VF.fadeField(cos(ofGetElapsedTimef() * 3.0f) * 2.0);
    
    
    for (int i = 0; i<50; i++) {
        vector<ofPolyline> polylines = paths[i].getOutline();
        for(int k = 0; k < polylines.size(); k++) {
            vector<ofPoint> vs = polylines[k].getVertices();
            for(int j = 1; j < vs.size(); j++) {
                ofPoint p = vs[j];
                float r = ofRandomf();
                if (r > 0.96) {
                    VF.addInwardCircle(p.x, p.y, 6, 0.6 * (1 + cos(ofGetElapsedTimef())));
                }
            }
        }
    }
}

void inflate(vector<ofPoint>& points, int target) {
    int n = points.size();
    if (n >= target) {
        // already sufficiant
        return;
    }
    if (n <= 1) {
        // TODO
        return;
    }
    
    int d = target - n;
    while (points.size() < target) {
        for (int i = 0; i < points.size() - 1; i++) {
            ofPoint p = (points[i] + points[i+1]) / 2.0;
            points.insert(points.begin() + i + 1, p);
            i++;
            if (points.size() == target) {
                return;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //現在のモードを画面に表示
    string msg;
    switch (addMode) {
            
        case 1:
            msg = "mode : Outward";
            break;
            
        case 2:
            msg = "mode : Inward";
            break;
            
        case 3:
            msg = "mode : Clockwise";
            break;
            
        case 4:
            msg = "mode : Counter Clockwise";
            break;
            
        default:
            break;
    }
    ofSetColor(255, 255, 255);
//    ofDrawBitmapString("key 1 - 4 : change vector mode", 20, 20);
//    ofDrawBitmapString(msg, 20, 40);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //ベクトル場を描画
    ofSetColor(0,130,130, 127);
    VF.draw();
    
    //ベクトル場に配置されたparticleを描画
    ofSetColor(0, 127, 255)	;
    for (int i = 0; i < particles.size(); i++){
        particles[i].draw();
    }	
    
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        startTime = ofGetElapsedTimef();
        auto tmp = paths;
        dest = (dest + 1) % 3;
        paths = testFont.getStringAsPoints(sources[dest], 0, 0, 1104);
        paths2 = testFont.getStringAsPoints(sources[(dest + 1) % 3], 0, 0, 1104);
    }

    // 1 - 4のキーでベクトル場のモードを変更
    switch (key) {
        case '1':
            //外向き
            addMode = 1;
            break;
            
        case '2':
            //内向き
            addMode = 2;
            break;
            
        case '3':
            //時計回り
            addMode = 3;
            break;
            
        case '4':
            //反時計回り
            addMode = 4;
            break;
            
        case ' ':
            //パーティクルの場所を初期化
            for (int i = 0; i < particles.size(); i++){
                particles[i].setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
            }
            
        default:
            break;
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
    //モードに応じてベクトル場に力を加える
    switch (addMode) {
        case 1:
            //外向き
            VF.addOutwardCircle(x,y, 100, 0.2);
            break;
            
        case 2:
            //内向き
            VF.addInwardCircle(x,y, 100, 0.2);
            break;
            
        case 3:
            //時計回り
            VF.addClockwiseCircle(x, y, 100, 0.2);
            break;
            
        case 4:
            //反時計回り
            VF.addCounterClockwiseCircle(x, y, 100, 0.2);
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
