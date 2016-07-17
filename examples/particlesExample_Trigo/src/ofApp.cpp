#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 1500;
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 

	resetParticles();
	
	//GUi
	//required call
	gui.setup();
	
	//Interaction over the particles came from here
	myMetaInteration.setup();

}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the forth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}



//--------------------------------------------------------------
void ofApp::update(){
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update();
		
		//update our desired value for all the particles
		p[i].myAttracPos_ModeAtract = (myMetaInteration.point + myMetaInteration.center);
	}
	
	//lets add a bit of movement to the attract points
	for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
	
	myMetaInteration.update();
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
	ofSetColor(255);

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	if(bHideIndo){
		
		ofSetColor(190);
		if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
			for(unsigned int i = 0; i < attractPoints.size(); i++){
				ofNoFill();
				ofDrawCircle(attractPointsWithMovement[i], 10);
				ofFill();
				ofDrawCircle(attractPointsWithMovement[i], 4);
			}
		}
		
		ofSetColor(230);
		ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);
		
		
		
		//
		drawGui();
		
		
		myMetaInteration.draw();
	}
}

//-------------------------------------------------------------
void ofApp::drawGui(){
	
	//required to call this at beginning
	gui.begin();
	
	ImGui::Checkbox("Draw trigo", &myMetaInteration.bDrawTrigo);
	ImGui::SliderInt("Radius", &myMetaInteration.radius, -1000, 1000);
	if(ImGui::SliderFloat("amplitude radius", &myMetaInteration.myInteractiveOscillation.amplitude, 0, 1000)){
		
		//myMetaInteration.amplitude +=
		//myMetaInteration.freq
	}
	
	ImGui::SliderFloat("Freq radius", &myMetaInteration.myInteractiveOscillation.freq, 0, 0.2);
	
	
	//required to call this at end
	gui.end();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 						
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}	
		
	if( key == ' ' ){
		resetParticles();
	}
	
	if( key == 'h' ){
		bHideIndo = !bHideIndo;
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