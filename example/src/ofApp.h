#pragma once

#include "ofMain.h"

#include "ofxDrawableStateMachine.h"




class DummyContentManager{
public:
	void setup(){}
	void fetchContent(){}
	void update(){};
	float getPercentDone(){return fmod(ofGetElapsedTimef(), 1);}
	bool isBusy(){return true;}
	bool isContentReady(){return true;}
	bool foundError(){return false;}
};


const float loadJsonTime = 6.0;
const float jsonFailedTime = 6.0;

class ofApp : public ofBaseApp{

public:

	enum AppState{
		LOADING_CONTENT = 0,
		LOADING_CONTENT_FAILED,
		RUNNING
	};

	void setup();
	void update();
	void draw();

	void keyPressed(int key){};
	void keyReleased(int key){};

	// APP CALLBACKS ////////////////////////////////////////

	void stateChanged(ofxStateMachine<AppState>::StateChangedEventArgs& change);
	void stateError(ofxStateMachine<AppState>::ErrorStateEventArgs& error);

	// APP SETUP ////////////////////////////////////////////

	//State Machine
	ofxDrawableStateMachine<AppState> dsm;

	DummyContentManager contentManager;
};
