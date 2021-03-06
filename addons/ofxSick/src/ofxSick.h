#pragma once

#include "ofMain.h"
#include "LMS1xx.h"

class PulseData {
public:
	vector<unsigned short> distance, brightness;
};

class ScanData {
public:
	PulseData first, second;
};

class ofxSick : public ofThread {
public:
	ofxSick();

	void setup();
	void update();
	bool isFrameNew();
	void draw(int gridDivisions = 10, float gridSize = 2000);
	void drawRadarLines();
	
	void setScanningFrequency(float scanningFrequency);
	void setAngularResolution(float angularResolution);
	void setAngleRange(float startAngle, float stopAngle);
	void setInvert(bool invert);
	void setAngleOffset(float angleOffset);
	float getAngleOffset() const;
	
	const vector<unsigned short>& getDistanceFirst() const;
	const vector<unsigned short>& getBrightnessFirst() const;
	const vector<unsigned short>& getDistanceSecond() const;
	const vector<unsigned short>& getBrightnessSecond() const;
	
	const vector<ofVec2f>& getPointsFirst() const;
	const vector<ofColor>& getColorsFirst() const;
	const vector<ofVec2f>& getPointsSecond() const;
	const vector<ofColor>& getColorsSecond() const;
	
protected:
	bool newFrame;
	bool invert;
	
	float scanningFrequency, startAngle, stopAngle, angularResolution;
	
	virtual void analyze();
	void polarToCartesian(vector<unsigned short>& polar, vector<ofVec2f>& cartesian) const;
	void brightnessToColor(vector<unsigned short>& brightness, vector<ofColor>& color) const;
	
	float angleOffset;
	ScanData scanBack, scanFront;
	vector<ofVec2f> pointsFirst, pointsSecond;
	vector<ofColor> colorsFirst, colorsSecond;
};

class ofxSickGrabber : public ofxSick {
public:
	ofxSickGrabber();
	void setIp(string ip);
	void startRecording();
	void stopRecording(string filename);
	
protected:
	LMS1xx laser;
	string ip;
	
	bool recording;
	vector<ScanData> recordedData;
	
	void connect();
	void disconnect();
	void threadedFunction();
	void analyze();
	void confirmCfg(int curCfg, int targetCfg, const string& name);
};

class ofxSickPlayer : public ofxSick {
public:
	ofxSickPlayer();
	void load(string filename);
	
protected:
	unsigned int position;
	vector<ScanData> recordedData;
	
	void threadedFunction();
};

