#pragma once
#include "ofMain.h"

//#include "TrigoInteraction.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};

class demoParticle{

	public:
		demoParticle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <ofPoint> * attract );

		void reset();
		void update();
		void draw();		
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector <ofPoint> * attractPoints;
	
	
	//For interaction
	ofPoint myAttracPos_ModeAtract;
	//setters
	
	void setAttracPos_ModeAtract(ofPoint _point);
	ofPoint getAttracPos_ModeAtract();
};