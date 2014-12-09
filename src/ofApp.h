#pragma once

#include "ofMain.h"
#include "ofxArtnet.h"

#define NB_STRIPS 10
#define HALF 300


float adist(float a1, float a2, bool rad=false);

class LightStrip{
public:
    
    int id_cold;
    int id_warm;
    
    LightStrip(int start_id){
        id_warm=start_id;
        id_cold=start_id+1;
    }
    
    void setLight(unsigned char dmxData[512],float temp,float intensity){
        dmxData[id_warm-1]=intensity*(temp+50)/100;
        dmxData[id_cold-1]=intensity*(50-temp)/100;
        
        ofSetColor(20+(205+3*temp/5)*intensity/255,20+205*intensity/255,20+(205-3*temp/5)*intensity/255);
        ofFill();
        ofRect(-50,NB_STRIPS*100/(2*PI),100,10);
    }
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxArtnet anNode;
    vector<LightStrip> strips;
    unsigned char dmxData[512];
    
    bool full_on=false;
    
    float angle=0;
    float intensity=0;
    float spread=100;
    float temp=0;
    
    void controlLights(){
        float step=360/NB_STRIPS;
        float support=step+spread/2;
        
        if(full_on){
            for(int i=0;i<NB_STRIPS;i++){
                ofPushMatrix();
                ofRotate(step*(i+1/2));
                strips.at(NB_STRIPS-i-1).setLight(dmxData,temp,255);
                ofPopMatrix();
            }
        }else{
            for(int i=0;i<NB_STRIPS;i++){
                float lDist=abs(adist(angle,step*(i+1/2)));
                ofPushMatrix();
                ofRotate(step*(i+1/2));
                if(lDist<support){
                    float lIntensity=intensity*(1-lDist/(support));
                    strips.at(NB_STRIPS-i-1).setLight(dmxData,temp,lIntensity);
                }else{
                    strips.at(NB_STRIPS-i-1).setLight(dmxData,temp,0);
                }
                ofPopMatrix();
            }
        }
        
        this->anSendDMX();
    }
    
    void anSendDMX(){
        anNode.sendDmx("10.29.0.71", dmxData, 512);
    }
};