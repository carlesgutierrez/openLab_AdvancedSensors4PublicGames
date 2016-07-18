#include "ofxRadarGlxRss3.h"

ofxRadarGlxRss3::ofxRadarGlxRss3()
{
}

//------------------------------------------------
void ofxRadarGlxRss3::setup()
{
    url = "http://192.168.1.120/scan_radars";

	timerReader = ofGetElapsedTimeMillis();

	ofLogVerbose(OF_LOG_SILENT);

	setupRadar();

	//TODO Set a theme
	ofSetBackgroundColor(50, 50, 50);

}

//------------------------------------------------
void ofxRadarGlxRss3::setupRadar() {
	sensorMaxDistance = 100;
	angleMaxRadar = 80;//Seems that now can be 80. But only 40 effectively-
	cout << "Radar Accurate Angle is = " << angleMaxRadar*0.5 << " but the maximum angle detection is " << angleMaxRadar << endl;
	ofVec2f posYMaxAccurateAngle = transformPolarToCartesian(sensorMaxDistance, angleMaxRadar*0.25);
	ofVec2f posYMaximumAngle = transformPolarToCartesian(sensorMaxDistance, angleMaxRadar);

	sensorHeight = posYMaximumAngle.y * 2; // posYMaximumAngle is only half upper of the radar 
	sensorAccurateHeight = posYMaxAccurateAngle.y * 2; // posYMaxAccurateAngle is only half upper of the radar
	sensorWidth = sensorMaxDistance;

	defineIdealRadarArea();

}

//------------------------------------------------
void ofxRadarGlxRss3::update() {

	if (bPlaying) {
		//Playing Simulation data if its available
		if (ofGetElapsedTimeMillis() - timerReader > READABLE_FRAMERATE) {
			readJsonDataRadar();
			timerReader = ofGetElapsedTimeMillis();
			radarPolarToCartesian();
		}
	}
	else {
		//Trying to get Json Data from Server 192.168.1.x
		updateOnlineRadarData();
	}

	

}

void ofxRadarGlxRss3::updateOnlineRadarData() {
	if (parsingSuccessful)
	{
		//readJSon data
		if (ofGetElapsedTimeMillis() - timerReader > READABLE_FRAMERATE) {
			readJsonDataRadar();
			timerReader = ofGetElapsedTimeMillis();
			radarPolarToCartesian();

			if (bRecording)updateRecording();
		}
	}
	else {
		//else not connected try it againg each 5 seconds
		//or playing another data
		if (ofGetElapsedTimeMillis() - timerReader > RECONNECTING_TIME) {
			readJsonDataRadar();
			timerReader = ofGetElapsedTimeMillis();
		}
	}
}

//------------------------------------------------
void ofxRadarGlxRss3::parseJsonDataRadar() {
	// Now parse the JSON
	parsingSuccessful = jsonRadar.open(url);

	if (parsingSuccessful)
	{
		ofLogNotice("ofxRadarGlxRss3::setup") << jsonRadar.getRawString(true);
	}
	else {
		ofLogNotice("ofxRadarGlxRss3::setup") << "Failed to parse JSON.";
	}
}

//------------------------------------------------
void ofxRadarGlxRss3::parseJsonDataRadar(ofxJSONElement _jsonRadar) {
	// Auto parsed success
	parsingSuccessful = true;

	jsonRadar.clear(); //Clear last frame datat
	jsonRadar = _jsonRadar; //Save the new one

}

//------------------------------------------------
void ofxRadarGlxRss3::readJsonDataRadar() {

	//Auto clean if no new data
	targetsData.clear();

	//Filll TargetsData with onlin
	if (bPlaying) {//TODO Set bSimulationMode
		parseJsonDataRadar(readingSimulationRadar());
	}
	else {
		parseJsonDataRadar();
	}

	if (jsonRadar["targets"].size() > 0) {
		
		targetsData.reserve(jsonRadar["targets"].size());

		for (Json::ArrayIndex i = 0; i < jsonRadar["targets"].size(); ++i){
			
			targetData auxTarget;
			auxTarget.id = jsonRadar["targets"][i]["id"].asInt64();
			auxTarget.distance = jsonRadar["targets"][i]["distance"].asInt();
			auxTarget.angleDegree = jsonRadar["targets"][i]["angle"].asInt();
			auxTarget.speedKm = jsonRadar["targets"][i]["speed"].asInt();
			auxTarget.strength = jsonRadar["targets"][i]["strength"].asInt();
			
			targetsData.push_back(auxTarget);
		}
	}

	//Any way this is always visible and should update timeStamp value properly ( try to find optimized frameRate reading )

	int timestamp = jsonRadar["timestamp"].asInt();
	std::string json_units = jsonRadar["json_units"].asString();
	std::string ui_units = jsonRadar["ui_units"].asString();
	textStatusRadar = "TimeStamp[" + ofToString(timestamp, 0) + "] - json_units[" + json_units + "] - ui_units[" + ui_units + "]";
		


}

//------------------------------------------------
void ofxRadarGlxRss3::draw()
{
  
	ofSetColor(ofColor::white);

	if (parsingSuccessful){

		drawRawTextRadarInfo();
		drawRawTextRadarDetection();

		drawIdealAreaTracking(ofGetWidth()*0.5, ofGetHeight()*0.75);
		drawBlobsCartesian(ofGetWidth()*0.5, ofGetHeight()*0.75);

	}
	else {
		ofDrawBitmapString("Parsing not succsefull. Reconnecting in " + ofToString(RECONNECTING_TIME - (ofGetElapsedTimeMillis() - timerReader), 0), 20, 40);
	}
	
}


//---------------------------------------------
void ofxRadarGlxRss3::defineIdealRadarArea() {

	ofPoint cornerPoint = ofPoint(0, sensorHeight*0.5);

	ofVec2f UpArc = transformPolarToCartesian(sensorMaxDistance, angleMaxRadar*0.25);
	ofVec2f DownArc = transformPolarToCartesian(sensorMaxDistance, -angleMaxRadar*0.25);


	radarAreaEffective.addVertex(cornerPoint);
	radarAreaEffective.addVertex(UpArc);
	radarAreaEffective.addVertex(DownArc);
	radarAreaEffective.close();

		//polylines[polylinesIndex].addVertex(ofPoint(args.x - imageRecognitionPosition.x, args.y - imageRecognitionPosition.y));
}

//---------------------------------------------
void ofxRadarGlxRss3::drawIdealAreaTracking(int _x, int _y) {
	ofPushMatrix();
	ofTranslate(_x, _y, 0);
	ofScale(sensorScale, sensorScale, 0);

	ofNoFill();
	
	ofSetColor(ofColor::orangeRed);
	ofDrawRectangle(0, 0, sensorMaxDistance, sensorHeight);
	
	ofSetColor(ofColor::yellow);
	ofDrawRectangle(0, sensorHeight*0.5 - sensorAccurateHeight*0.5, sensorMaxDistance, sensorAccurateHeight);


	ofSetColor(ofColor::forestGreen);
	radarAreaEffective.draw();
	ofPopMatrix();
}

//---------------------------------------------
void ofxRadarGlxRss3::drawBlobsCartesian(int _x, int _y) {

	ofPushMatrix();
	ofTranslate(_x, _y, 0);
	ofScale(sensorScale, sensorScale, 0);

	ofFill();
	ofColor myBlobColor = ofColor::mediumVioletRed;
	ofEnableAlphaBlending();
	ofSetColor(myBlobColor.r, myBlobColor.g, myBlobColor.b, 200);
	for (int i = 0; i < cartesianRadar.size(); i++) {
		ofDrawCircle(cartesianRadar[i].x, cartesianRadar[i].y, 5);
		ofDrawBitmapString("x="+ofToString(cartesianRadar[i].x, 0), cartesianRadar[i].x+2, cartesianRadar[i].y+0);
		ofDrawBitmapString("y="+ofToString(cartesianRadar[i].y, 0), cartesianRadar[i].x+2, cartesianRadar[i].y+5);
	}
	ofDisableAlphaBlending();
	ofPopMatrix();
}

//---------------------------------------------
void ofxRadarGlxRss3::drawRawTextRadarDetection() {

	ofSetColor(ofColor::indianRed);
	int gapInfoLines = 40;
	int gapInfoTargetInfo = 120;

	//Draw all Targets Info
	for (int i = 0; i < targetsData.size(); i++) {
		ofDrawBitmapString("id = " + ofToString(targetsData[i].id, 2), 20, i * gapInfoTargetInfo + gapInfoLines + 20);
		ofDrawBitmapString("distance = " + ofToString(targetsData[i].distance, 2), 20, i * gapInfoTargetInfo + gapInfoLines + 40);
		ofDrawBitmapString("angleDegree = " + ofToString(targetsData[i].angleDegree, 2), 20, i * gapInfoTargetInfo + gapInfoLines + 60);
		ofDrawBitmapString("speedKm = " + ofToString(targetsData[i].speedKm, 2), 20, i * gapInfoTargetInfo + gapInfoLines + 80);
		ofDrawBitmapString("strength = " + ofToString(targetsData[i].strength, 2), 20, i * gapInfoTargetInfo + gapInfoLines + 100);
		ofDrawBitmapString("---------------------", 20, i * gapInfoTargetInfo + gapInfoLines + 120);
	}
}

//-----------------------------------------------
void ofxRadarGlxRss3::drawRawTextRadarInfo() {

	ofSetColor(ofColor::green);
	//Draw Basic Info
	ofDrawBitmapString(textStatusRadar, 20, 20);

	ofSetColor(ofColor::red);
	//Draw Resumed Blob Data
	ofDrawBitmapString(stargetResumedData = "Num targets= " + ofToString(jsonRadar["targets"].size(), 0), 20, 40);

	//Some extra info
	ofSetColor(ofColor::whiteSmoke);
	//ofDrawBitmapString("ofGetElapsedTimef =" + ofToString(ofGetElapsedTimef(), 4), 500, 30);
	ofDrawBitmapString("Framerate =" + ofToString(ofGetFrameRate(),2), 500, 30);

	ofSetColor(ofColor::red);
		
	// Show the current mouse recording state
	if(bRecording){
		ofDrawBitmapString("Recording Radar", 500, 100);
	}
	else if (bPlaying) {
		ofDrawBitmapString("Playing OffLine Radar "+ fileName, 500, 80);
		ofDrawBitmapString("Num FramesSimulation = " +ofToString(counterFramesSimulation, 0) , 500, 100);
		ofDrawBitmapString("Total Frames available = " + ofToString(jsonSimulationRadar.size(), 0), 500, 120);
	}

	

}

//-----------------------------------------------
void ofxRadarGlxRss3::radarPolarToCartesian() {
	//Auto Clean cartesian if there is no new targetsData
	cartesianRadar.clear();
	cartesianRadar.reserve(targetsData.size());

	for (int i = 0; i < targetsData.size(); i++) {
		
		ofVec2f auxpoint = transformPolarToCartesian(targetsData[i].distance, targetsData[i].angleDegree);
		cartesianRadar.push_back(auxpoint);
	}


}

//--------------------------------------------------------------
ofVec2f ofxRadarGlxRss3::transformPolarToCartesian(float _distance, float _angleDegree) {

	float auxX;
	float auxY;

	if (_angleDegree > 0) {
		float degreeRad = ofDegToRad(abs(_angleDegree));

		auxX = _distance * cos(degreeRad);
		auxY = _distance * sin(degreeRad);

		//Then apply Radar adaptation ( positive angle )
		auxY = - auxY + sensorHeight*0.5;
		

		//cout << "cartesian x = " << auxX << " from _distance= " << _distance << " and positive angle=" << _angleDegree << endl;
		//cout << "cartesian y = " << auxY << " from _distance= " << _distance << " and positive angle=" << _angleDegree << endl;

}
	else {
		
		float degreeRad = ofDegToRad(abs(_angleDegree));

		auxX = _distance * cos(degreeRad);
		auxY = _distance * sin(degreeRad);

		//Then apply Radar adaptation ( negative angle )
		auxY = auxY + sensorHeight * 0.5;

		//cout << "cartesian x = " << auxX << " from _distance= " << _distance << " and negative angle= -" << _angleDegree << endl;
		//cout << "cartesian y = " << auxY << " from _distance= " << _distance << " and negative angle= -" << _angleDegree << endl;
	}
	
	return ofVec2f(auxX, auxY);

}

//-------------------------------------------------------
void ofxRadarGlxRss3::updateRecording() {

	if (bRecording) {

		/*
		//When has no targets:
		{"targets":[],"timestamp":1412,"json_units":"kmh","ui_units":"kmh"}
		//When has targets:
		{"targets":[{"id":137693,"distance":35.7982,"angle":15.8591,"speed":-0.55,"strength":1.06788e+07}],"timestamp":1595,"json_units":"kmh","ui_units":"kmh"}
		*/

		ofxJSONElement saverBlobJS;
		saverBlobJS = jsonRadar;

		jsonRecordingRadar.append(saverBlobJS);
	}

}

//-------------------------------------------------------
void ofxRadarGlxRss3::startRecorging() {

	if (!bStartRecording) {
		bStartRecording = true;

		jsonRecordingRadar.clear();
		framesRecorded = 0;
		bRecording = true;
	}
	else {
		cout << "Error action. Was already Reconding. Stop Recording fisrt, press t" << endl;
	}
	
}

//-------------------------------------------------------
void ofxRadarGlxRss3::stopRecorging() {

	if (bRecording) {
		jsonRecordingRadar.save("radar_" + ofGetTimestampString()+".json", false); //true for readable or False for faster
		bRecording = false;
		bStartRecording = false;
	}
	else if (!bStartRecording) {
		bStartRecording = true;
	}
	else {
		cout << "Error action. Was already Reconding. Stop Recording fisrt, press t" << endl;
	}


}

//-------------------------------------------------------
void ofxRadarGlxRss3::startPlaying() {
	//jsonRecordingRadar.

	if (bPlaying) {
		//Cameback To Radar
		bPlaying = false;
		bStartPlaying = false;
	}
	else if (bStartRecording || bRecording) {
		bStartPlaying = false;
		bPlaying = false;
	}
	else {
		//!bPlaying case
		if (!bStartPlaying) {
			counterFramesSimulation = 0;
			jsonSimulationRadar.clear();
			bool bOpen = jsonSimulationRadar.openLocal(fileName);
			if (bOpen) {
				bStartPlaying = true;
				bPlaying = true;
				//Save Total Frames to do loop over this ones
				totalFramesSimulation = jsonSimulationRadar.size();
			}
			else {
				cout << "startPlaying:: Error opening Simulation radar.json" << endl;
			}
			
		}
		
	}
}

//-------------------------------------------------------
//Forcing easy sequencial reading
ofxJSONElement ofxRadarGlxRss3::readingSimulationRadar() {

	ofxJSONElement auxReadFrame;

	if (counterFramesSimulation > totalFramesSimulation) {
		counterFramesSimulation = 0;
	}
	else {
		counterFramesSimulation++;
	}

	auxReadFrame = jsonSimulationRadar[counterFramesSimulation];

	cout << "My new Simulation Frame is " << auxReadFrame << endl;
	
	return auxReadFrame;
}

//-------------------------------------------------------
bool ofxRadarGlxRss3::isPlaying() {
	return bPlaying;
}

//-------------------------------------------------------
bool ofxRadarGlxRss3::isRecording() {
	return bRecording;
}

//------------------------------------------------------
void ofxRadarGlxRss3::playSimFile(int _idPosFileInFolder) {
	myDataFolder.listDir("");
	myDataFolder.sort();

	if (_idPosFileInFolder < myDataFolder.size() && _idPosFileInFolder > -1) {
		idActualSimulatorIter = _idPosFileInFolder;
		fileName = myDataFolder.getPath(idActualSimulatorIter);
		startPlaying();
	}
	
}

//------------------------------------------------------
int ofxRadarGlxRss3::getNumSimFiles() {
	myDataFolder.listDir("");
	myDataFolder.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	return myDataFolder.size();
}


//
//Map Strengt Into Out Circle Size no Fill
//Map velocity into Variables
