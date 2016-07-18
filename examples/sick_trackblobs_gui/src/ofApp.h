#pragma once

#include "ofMain.h"
#include "ofxSick.h"
#include "ofxSickTracker.h"
#include "ofxCv.h"
#include "ofxImGui.h"
#include "ofxOsc.h"

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
	int maximumDistance = 100;
	int persistence = 10;
	ofRectangle trackingRegion;
	
	//TrackerFollower control vars
	bool bKmeans = true;
	ofPoint myRegionPos;
	ofPoint myRegionSize;
	
	//This values can be good for hand/arm tracking
	//but not for body or far away detected clusters with few points
	//Use Sliders setup values.
	
	float maxStddev = 60;
	int maxClusterCount = 100;
	
	int minClusterSize = 40;
	int maxPointDistance = 100;
	
	//Gui
	void drawGui_OSC_configurable();
	void drawGui();
	ofxImGui myGui;
	
	//OSC will send all valuable data to others
	void sendOSCBlobTrackerData();
	bool bOSCActived = false;
	string IPSender = "127.0.0.1";
	int PortSender = 4000;
	ofxOscSender sender;
	bool bResetHostIp = false;
	
	//Data Tracked
	vector<cv::Point2f> currentTrackPoints;
	bool bDrawClusters = false;
};