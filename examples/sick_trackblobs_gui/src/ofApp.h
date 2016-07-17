#pragma once

#include "ofMain.h"
#include "ofxSick.h"
#include "ofxSickTracker.h"
#include "ofxCv.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofxSickGrabber grabber;
	ofxSickPlayer player;
	ofxSick* sick;
	bool recording;
	float scale = 0.1;
	
	ofxSickTracker<ofxSickFollower> tracker;
	int maximumDistance = 100;//Supouse to be good values for sick detection
	int persistence = 10;
	ofRectangle trackingRegion;
	
	//control vars
	bool bKmeans = true;
	ofPoint myRegionPos;
	ofPoint myRegionSize;
	
	float maxStddev = 60;
	int maxClusterCount = 100;
	
	int minClusterSize = 40;
	int maxPointDistance = 100;
	
	
	void drawGui();
	ofxImGui myGui;
};