#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	ofSetCircleResolution(64);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	recording = false;
	
	grabber.setup();
	grabber.setIp("169.254.206.112");
	//player.load("recording.lms");
	
	sick = &grabber;//&player;
	
	trackingRegion.set(-630, 200, 1200, 1600);
	myRegionPos.x = trackingRegion.getX();
	myRegionPos.y = trackingRegion.getY();
	myRegionSize.x = trackingRegion.getWidth();
	myRegionSize.y = trackingRegion.getHeight();
	
	tracker.setMaximumDistance(maximumDistance);
	tracker.setPersistence(persistence);
	tracker.setRegion(trackingRegion);
	
	//required call
	myGui.setup();
}

void ofApp::update() {
	sick->update();
	if(sick->isFrameNew()) {
		tracker.update(*sick);
	}
}

void ofApp::draw() {
	ofBackground(0);
	
	//float scale = ofMap(mouseX, 0, ofGetWidth(), 0.05, .2, true);
	
	ofPushMatrix();
	ofTranslate(20, 20);
	ofSetColor(255);
	ofDrawCircle(0, 0, 7);
	ofDrawBitmapString(ofToString(tracker.size()) + " clusters", -4, 4);
	ofPopMatrix();
	
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	
	ofPushMatrix();
	ofScale(scale, scale);
	sick->draw(12, 2400);
	tracker.draw();
	ofPopMatrix();
	
	//ofDrawString("Sick data Frms")
	
	drawGui();
}

//---------------------------------------------------------------
void ofApp::drawGui(){
	myGui.begin();
	
	
	ImGui::SliderFloat("Scale Area", &scale, 0.05, .2);

	if(ImGui::SliderFloat("set X Area tracker", &myRegionPos.x, -10000, 10000)){
		trackingRegion.setX(myRegionPos.x);
		tracker.setRegion(trackingRegion);
	}
	if(ImGui::SliderFloat("set Y Area tracker", &myRegionPos.y, -10000, 10000)){
		trackingRegion.setY(myRegionPos.y);
		tracker.setRegion(trackingRegion);
	}
	if(ImGui::SliderFloat("set Width Area tracker", &myRegionSize.x, 0, 5000)){
		trackingRegion.setWidth(myRegionSize.x);
		tracker.setRegion(trackingRegion);
	}
	if(ImGui::SliderFloat("set Height Area tracker", &myRegionSize.y, 0, 5000)){
		trackingRegion.setHeight(myRegionSize.y);
		tracker.setRegion(trackingRegion);
	}
	
	if(ImGui::Checkbox("Kmeans active", &bKmeans)){
		if(bKmeans)tracker.setupKmeans(maxStddev, maxClusterCount);
		else tracker.setupNaive(minClusterSize, maxPointDistance);
	}

	if(ImGui::SliderInt("set maximum Distance tracker", &maximumDistance, 10, 5000)){
		tracker.setMaximumDistance(maximumDistance);
	}
	if(ImGui::SliderInt("set Persistence tracker", &persistence, 2, 500)){
		tracker.setPersistence(persistence);
	}
	

	if(ImGui::SliderFloat("set maxStddev tracker kmeans", &maxStddev, 10, 1000)){
		tracker.setupKmeans(maxStddev, maxClusterCount);
		bKmeans = true;
	}
	if(ImGui::SliderInt("set maxClusterCount tracker kmeans", &maxClusterCount, 2, 500)){
		tracker.setupKmeans(maxStddev, maxClusterCount);
		bKmeans = true;
	}

	if(ImGui::SliderInt("set minClusterSize tracker naive", &minClusterSize, 10, 1000)){
		tracker.setupNaive(minClusterSize, maxPointDistance);
		bKmeans = false;
	}
	if(ImGui::SliderInt("set maxPointDistance tracker naive", &maxPointDistance, 10, 1000)){
		tracker.setupNaive(minClusterSize, maxPointDistance);
		bKmeans = false;
	}
	
								
	
	myGui.end();
}

void ofApp::keyPressed(int key){
	if(key == 'r') {
		recording = !recording;
		if(recording) {
			grabber.startRecording();
		} else {
			grabber.stopRecording("out.lms");
		}
	}
	if(key == '\t') {
		if(sick == &player) {
			sick = &grabber;
		} else {
			sick = &player;
		}
	}
}
