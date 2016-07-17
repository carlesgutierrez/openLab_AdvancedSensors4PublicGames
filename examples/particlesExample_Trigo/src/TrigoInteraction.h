//
//  TrigoInteraction.h
//  particlesExample_Trigo
//
//  Created by carles on 17/07/16.
//
//

#pragma once


#include "ofMain.h"

//The following defines are called preprocessor directives.
//This are just to make the programmers life easier.
//This are actually not compiled, instead whenever you use a defined directive it gets replaced by it's associated value in the source code BEFORE it compiles.
//So this don't use memory space and if you are using this directives a lot it's easier and faster to change it's value.
#define TOP_MARGIN 150
#define LEFT_MARGIN 150

#define WAVEFORM_HISTORY 800
#define TAIL_LENGTH 400

//this is our oscillator class.
//This defines how to save, update and draw each oscillators properties and values.
class  oscillator {
public:
	
	float freq;
	float waveSin;
	float waveCos;
	float amplitude;
	float phase;
	double counter;
	
	ofVec2f pos;
	
	oscillator(){}
	~oscillator(){}
	void setup(int x, int y){//this function initializes all the properties of the oscillator. It must be called just after creating a new oscillator.
		
		pos.set(x, y);//sets the position of the oscillator using the values passed to the function. In this case is the position where the mouse was clicked.
		freq = ofRandom(0.0001f, 0.1f);// the frequency get's initialized with a random value.
		
		counter = 0;
		if (y<TOP_MARGIN) {//this if is to determine if the oscillator is either an horizontal or vertical oscillator.
			phase = ofMap(x-LEFT_MARGIN, LEFT_MARGIN, ofGetWidth(), -PI, PI, false);//the phase is defined by to position of the locator.Here it's value gets remaped to the correct range.
			
			amplitude = ofMap(y, 0, TOP_MARGIN, 0.1f, 300, false);//Amplitude is defined by the position, and it's value gets remaped to the correct range.
			
		}else {
			phase = ofMap(y-TOP_MARGIN, TOP_MARGIN, ofGetHeight(), -PI, PI, false);
			amplitude = ofMap(x, 0, LEFT_MARGIN, 0.1f, 300, false);
		}
	}
	
	void update(){
		counter += freq;//the counter is incremented by the value of freq each time the oscillators update methods is called.
		waveSin = sin(counter + phase) * amplitude;//Here is where we calulate the sine value for the oscillator.
		//Then is multiplied by amplitude, because the sin() function returns a value between -1 and 1
		waveCos = cos(counter + phase) * amplitude;
	}
	
	void draw(){//here we draw the oscillator
		ofPushStyle();
		ofFill();
		ofDrawCircle(pos.x, pos.y, amplitude/4);
		ofSetColor(40);
		ofNoFill();
		ofDrawCircle(pos.x, pos.y, amplitude/4);
		ofDrawLine(pos.x, pos.y, pos.x +  waveCos/4, pos.y + waveSin/4);
		ofPopStyle();

	}
	bool checkOver(int x, int y){//this is to check if the mouse is over the oscillator.
		if (pos.distance(ofVec2f(x,y)) <amplitude*0.25) {
			return true;
		}else{
			return false;
		}
	}
};



class TrigoInteraction{
/*	
	// variables & methods for singleton
private:
	static bool	instanceFlag;
	static TrigoInteraction *single;
public:
	static TrigoInteraction* getInstance();
	// end singleton
	
*/
	//Then any where with only the include acess to Singleton access as:
	//TrigoInteraction::getInstance()->
public:

	
	void setup();
	void update();
	void draw();

	//Trigonometry vars and Func
	void updateTrigonometry();
	void drawTrigonometry();
	float rotationSpeed;
	int radius;
	float sine;
	float cosine;
	float angle;
	ofPoint center;

	
	ofPoint point;

	bool bDrawTrigo = true;
	oscillator myInteractiveOscillation;
	
};
