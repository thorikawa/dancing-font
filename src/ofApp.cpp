#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){    
    testFont.loadFont("GenShinGothic-Monospace-Regular.ttf", 18, false, true);
    
    
    sources.push_back(str);
    sources.push_back(str2);
    sources.push_back(str3);

    paths = testFont.getStringAsPoints(str, 0, 0, 1104);
    paths2 = testFont.getStringAsPoints(str2, 0, 0, 1104);
//    paths = testFont.getStringAsPoints(str);
//    paths2 = testFont.getStringAsPoints(str2);
    for (int i = 0; i<paths.size(); i++) {
        seeds.push_back(ofRandom(100.0));
    }
}

//--------------------------------------------------------------
void ofApp::update(){

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
    // we can also access the individual points
    ofSetColor(0, 90, 60);
//    ofNoFill();
    ofNoFill();
    ofTranslate(10, 40, 0);
    float t = ofGetElapsedTimef() - startTime;
    
    int count = 0;
    ofPushMatrix();
    for (int i1 = 0, i2 = 0; i1 < 1000 && i2 < 1000; i1++, i2++) {
        ofBeginShape();
        vector<ofPolyline> polylines = paths[i1].getOutline();
        vector<ofPolyline> polylines2 = paths2[i2].getOutline();
        while (polylines.size() == 0) {
            printf("polylines of %d is empty\n", i1);
            i1++;
            polylines = paths[i1].getOutline();
        }
        while (polylines2.size() == 0) {
            printf("polylines of %d is empty\n", i2);
            i2++;
            polylines2 = paths2[i2].getOutline();
        }
        int nline = max(polylines.size(), polylines2.size());
        for(int k = 0; k < nline; k++){
            if( k!= 0)ofNextContour(true) ;
            
            vector<ofPoint> vs;
            vector<ofPoint> vs2;
            
            if (polylines.size() < k + 1) {
                ofPoint p = polylines[0].getVertices()[0];
                vs.push_back(ofPoint(p.x, p.y));
                vs.push_back(ofPoint(p.x, p.y));
            } else {
                vs = polylines[k].getVertices();
            }
            if (polylines2.size() < k + 1) {
                ofPoint p = polylines2[0].getVertices()[0];
                vs2.push_back(ofPoint(p.x, p.y));
                vs2.push_back(ofPoint(p.x, p.y));
            } else {
                vs2 = polylines2[k].getVertices();
            }
            if (vs.size() < vs2.size()) {
                // increase the number of vs
                inflate(vs, vs2.size());
            } else if (vs.size() > vs2.size()) {
                inflate(vs2, vs.size());
            }
            int nvertex = max(vs.size(), vs2.size());
            for(int j = 0; j < nvertex; j++){
                float x, y, dx, dy;
                float wave = 0.0;
                if (vs.size() < j + 1) {
                    x = vs[0].x + sin(k + 0 + t * 20.0f) * wave;
                    y = vs[0].y;
                } else {
                    x = vs[j].x + sin(k + j + t * 20.0f) * wave;
                    y = vs[j].y;
                }
                
                if (vs2.size() < j + 1) {
                    dx = vs2[0].x;
                    dy = vs2[0].y;
                } else {
                    dx = vs2[j].x;
                    dy = vs2[j].y;
                }
//                float r = 1.0 - 1.0 / (1.0 + exp(-t / 20.0));
                float r = t / duration;
                if (r > 1.0f) {
                    r = 1.0f;
                }
                x = (1 - r) * x + r * dx;
                y = (1 - r) * y + r * dy;
                ofVertex(x, y);
                
                count++;
            }
        }
        ofEndShape(true);
    }
    ofPopMatrix();
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
