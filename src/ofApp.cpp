#include "ofApp.h"
#include "artnet.h"

//--------------------------------------------------------------


void ofApp::setup(){

    ofSetWindowShape(2*HALF,2*HALF);
    
    anNode.setup("10.0.0.4", 0xFD);
    
    for(int i = 0; i<512; i++) {
        dmxData[i] = 50;
    }
    this->anSendDMX();
    
    for(int i=0;i<NB_STRIPS;i++){
        strips.push_back(LightStrip(1+i*3));
    }

    
	ofSetCircleResolution(80);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(20);
    
    float xrel=mouseX-HALF;
    float yrel=mouseY-HALF;
    
    
    if(!xrel){
        if(yrel>0) angle=0;
        else if(yrel<0) angle=180;
    }else if(!yrel){
        if(xrel>0) angle=270;
        else if(xrel<0) angle=90;
    }else if(yrel>0){
        angle=-180*atan(xrel/yrel)/PI;
    }else{
        angle=180-180*atan(xrel/yrel)/PI;
    }
    
    intensity=sqrt(xrel*xrel+yrel*yrel);
    if(intensity>255) intensity=255;
    
    ofTranslate(HALF,HALF);
    
    ofSetColor(30+temp/5,30,30-temp/5);
    ofFill();
    ofCircle(0,0,intensity);
    
    ofSetColor(20);
    ofFill();
    
    ofRotate(angle);
    
    if(spread){
        float spreadLength=intensity*tan(PI*((360-spread)/4)/180);
        ofPushMatrix();
        ofRotate(spread/2+(360-spread)/4);
        ofTriangle(0,0,-spreadLength,intensity,spreadLength,intensity);
        ofPopMatrix();
        ofPushMatrix();
        ofRotate(-spread/2-(360-spread)/4);
        ofTriangle(0,0,-spreadLength,intensity,spreadLength,intensity);
        ofPopMatrix();
    }else{
        ofBackground(20);
    }
    
    ofRotate(-angle);
    
    this->controlLights();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 357: // up
            if(spread<360) spread+=5;
            break;
        case 359: // down
            if(spread>0) spread-=5;
            break;
        case 356: // left
            if(temp>-50) temp-=5;
            break;
        case 358: // right
            if(temp<50) temp+=5;
            break;
        case 32:  // space
            
            if(full_on) full_on=false;
            else full_on=true;
            break;
    }
    this->anSendDMX();

}


float adist(float a1, float a2, bool rad){
    float dist=a2-a1;
    if(rad){
        if(a2>a1 && (a1+PI-a2)<dist) dist=a2-a1-PI;
        if(a1>a2 && (a2+PI-a1)<-dist) dist=a2+PI-a1;
    }else{
        if(a2>a1 && (a1+360-a2)<dist) dist=a2-a1-360;
        if(a1>a2 && (a2+360-a1)<-dist) dist=a2+360-a1;
    }
    return dist;
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