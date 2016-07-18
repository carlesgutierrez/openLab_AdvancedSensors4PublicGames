#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myRadar.setup();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	myRadar.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	myRadar.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'r')myRadar.startRecorging();
	else if(key == 't')myRadar.stopRecorging();
	else if (key == 'p') {
		myRadar.startPlaying();
	}
	else if (key == '+') {
		idSimulaTorFile++;
		if(idSimulaTorFile < myRadar.getNumSimFiles()){
			myRadar.playSimFile(idSimulaTorFile);
		}
		else {
			idSimulaTorFile = 0;
			myRadar.playSimFile(idSimulaTorFile);
		}
		
	}
	
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
