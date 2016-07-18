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
	//Normally 1192.168.0.1 as default , but This Laser has been configured to this other IP.
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
	
	//imGui required call
	myGui.setup();
	
	// open an outgoing connection to HOST:PORT
	sender.setup(IPSender, PortSender);
	
}

//-----------------------------------------
void ofApp::update() {
	sick->update();
	if(sick->isFrameNew()) {
		tracker.update(*sick);
	}
	
	if(bResetHostIp){
		sender.setup(IPSender, PortSender);
		bResetHostIp = false;
	}
	
	if(bOSCActived){
		sendOSCBlobTrackerData();
	}
}

//-----------------------------------------
void ofApp::sendOSCBlobTrackerData(){
	
	currentTrackPoints.clear();
	currentTrackPoints = tracker.getClusters();
	//TODO check how to get the label of the cluster

	if(currentTrackPoints.size() > 0){
		ofxOscMessage m;
		m.clear();
		m.setAddress("/TrackBlobs");
		
		sender.sendMessage(m, false);
		
		float rectTrackerW = tracker.getTrackRegion().getWidth();
		float rectTrackerH = tracker.getTrackRegion().getHeight();
		
		for(int i=0 ; i < currentTrackPoints.size(); i++){
			ofPoint cluster = ofxCv::toOf(currentTrackPoints[i]);
			cluster.x = abs(tracker.getTrackRegion().getPosition().x - cluster.x);
			cluster.y = abs(tracker.getTrackRegion().getPosition().y - cluster.y);
			//TODO add label as m.addInt32Arg();
			//TODO Check if send X inverted and Y too. Xinverted = 1 - X
			m.addFloatArg(cluster.x/rectTrackerW);
			m.addFloatArg(cluster.y/rectTrackerH);
		}
		
		sender.sendMessage(m, false);
	}else{
		
		//TODO Check what is usefull to send too if no blobs are tracked
	}

	

	
}

//----------------------------------------
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
	
	
	//Manual Clusters Draw Method
	if(bDrawClusters){
		currentTrackPoints.clear();
		currentTrackPoints = tracker.getClusters();
		
		ofSetColor(0,0,255);
		
		for(int i=0 ; i < currentTrackPoints.size(); i++){
			ofPoint cluster = ofxCv::toOf(currentTrackPoints[i]);
			//TODO Check how to get the label from the cluster
			//ofDrawString("id = "+ofToString(labelCluster) , cluster.x, cluster.y - 40);
			ofDrawCircle(cluster.x, cluster.y, 40);
		}
	}else {
		tracker.draw();
	}
	
	ofPopMatrix();
	
	//ofDrawString("Sick data Frms")
	
	drawGui();
	
	
	//Draw and Edit OSC info
	//if(bOSCActived)drawGui_OSC_configurable();
}

//---------------------------------------------------------------
void ofApp::drawGui(){
	myGui.begin();
	
	if(recording){
		ImGui::Text("Sick REC !! ");
		//TODO Show seconds recordered
	}
	
	ImGui::PushItemWidth(150);
	
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
	
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();
	
	ImGui::Checkbox("Manual Clusters Drawer", &bDrawClusters);
	ImGui::Checkbox("Send OSC", &bOSCActived);
	
	ImGui::PopItemWidth();
	
								
	
	myGui.end();
}

//-------------------------------------------------
void ofApp::drawGui_OSC_configurable(){
	

	ImGui::Text("Sending OSC data to ");
	ImGui::Text(ofToString(PortSender,0).c_str());
	ImGui::Text(IPSender.c_str());
	
	//TODO InputTextFilterCharacter
	static char buf1[16] = "127.0.0.1";
	ImGui::PushItemWidth(90);
	ImGui::InputText("WIP Edit Host", buf1, 16);ImGui::SameLine();
	ImGui::Checkbox("Reset HOST IP", &bResetHostIp);
	ImGui::PopItemWidth();
	
	if(bResetHostIp){
		IPSender = std::string(buf1);
		cout << "Lets reset IPSender" << endl;
	}
	

}

void ofApp::keyPressed(int key){
	
	//As far as I tried
	//seems not working proplely at load and play this data
	//May be is necesary to set low or framerate reading ( sleepMillis  )
	//TODO debug timings
	
	
	if(key == OF_KEY_F1) {
		recording = !recording;
		if(recording) {
			grabber.startRecording();
		} else {
			grabber.stopRecording("out.lms");
		}
	}
	if(key == OF_KEY_F2) {
		if(sick == &player) {
			sick = &grabber;
		} else {
			sick = &player;
		}
	}
}
