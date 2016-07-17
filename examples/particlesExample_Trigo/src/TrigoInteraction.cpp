//
//  TrigoInteraction.cpp
//  particlesExample_Trigo
//
//  Created by carles on 17/07/16.
//
//

#include "TrigoInteraction.h"

/*
// SINGLETON initalizations
bool TrigoInteraction::instanceFlag = false;
TrigoInteraction* TrigoInteraction::single = NULL;

//----------------------------------------------
TrigoInteraction* TrigoInteraction::getInstance()
{
	if(! instanceFlag)
	{
		single = new TrigoInteraction();
		instanceFlag = true;
		return single;
	}else{
		return single;
	}
}


//----------------------------------------------
TrigoInteraction::TrigoInteraction()
{
	
}
//----------------------------------------------
TrigoInteraction::~TrigoInteraction()
{}
*/

//--------------------------------------------------------------
void TrigoInteraction::setup(){
	
	//setup Trigo
	ofSetCircleResolution(40);
	angle=0;
	cosine=0;
	sine=0;
	rotationSpeed=0.01f;
	radius = 180;
	center = ofPoint(ofGetWidth()*0.5, ofGetHeight()*0.5);
	
	myInteractiveOscillation.setup(800, 50);
	
}

//--------------------------------------------------------------
void TrigoInteraction::update(){
	
	updateTrigonometry();
	
}
//--------------------------------------------------------------
void TrigoInteraction::updateTrigonometry(){
	
	angle+=rotationSpeed;//at each update the angle get's incremented
	
	//here we get the sine and cosine values for the angle
	cosine=cos(angle);
	sine=sin(angle);
	
	point.set(cosine * radius, sine * radius, 0);//here we set the cyan circle position
	
	//Tryin to move this point after with some extra Interaction
	radius = myInteractiveOscillation.waveSin;
	
	//Apply wied noise to the radius
	myInteractiveOscillation.update();
	
}

//--------------------------------------------------------------
void TrigoInteraction::draw(){
	if(bDrawTrigo){
		drawTrigonometry();
		myInteractiveOscillation.draw();
		
		ofDrawBitmapString("radius = " + ofToString(radius,2), 700, 900);
	}
}


//--------------------------------------------------------------
void TrigoInteraction::drawTrigonometry(){
	ofPushMatrix();
	ofTranslate(center);
	
	ofEnableAlphaBlending();
	//draw the yellow circle
	ofSetColor(240, 230, 10, 100);
	ofDrawCircle(0,0, radius);
	ofDisableAlphaBlending();
	
	//draw the cyan circle that's moving over the yellow circle's perimeter
	ofSetColor(0, 140, 255);
	ofDrawCircle(point, 10);
	
	ofSetLineWidth(3);
	//draw lines from the cyan ball to the pink bars
	ofSetColor(255, 0, 50);
	ofDrawLine(point.x, point.y, cosine * radius, -radius * 2);
	ofDrawLine(point.x, point.y, radius * 2, sine * radius);
	
	ofSetLineWidth(1);
	
	ofPopMatrix();
}