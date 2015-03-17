#pragma once

#include "ofMain.h"


#include "ofxStateMachine.h"
#include "ofxStateMachine.cpp" //NOTE THE INCLUSION OF .CPP!! Bc of the templated code
//http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file


class ofApp : public ofBaseApp{

public:

	enum AppState{
		LOADING_ASSETS = 0,
		LOADING_JSON,
		JSON_FAILED,
		STARTUP_TESTS,
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

	ofxStateMachine<AppState> state;

	bool loadJson();
};
