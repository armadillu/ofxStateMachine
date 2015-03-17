#include "ofApp.h"


void ofApp::setup(){

	ofBackground(22);
	ofSetFrameRate(60);

	//listen to state machine changes
	ofAddListener(state.eventStateChanged, this, &ofApp::stateChanged);
	ofAddListener(state.eventStateError, this, &ofApp::stateError);

	//init state names
	state.SET_NAME_FOR_STATE(LOADING_ASSETS);
	state.SET_NAME_FOR_STATE(LOADING_JSON);
	state.SET_NAME_FOR_STATE(STARTUP_TESTS);
	state.SET_NAME_FOR_STATE(RUNNING);

	//set initial state;
	state.setState(LOADING_ASSETS);
}


void ofApp::stateChanged(ofxStateMachine<AppState>::StateChangedEventArgs& change){
	ofLog() << "State Changed from " << state.getNameForState(change.oldState) << " to "
	<< state.getNameForState(change.newState);
}


void ofApp::stateError(ofxStateMachine<AppState>::ErrorStateEventArgs& error){
	ofLog() << "Error '" << error.errorMsg << "' during state '" << state.getNameForState(error.state) << "'";
}



bool ofApp::loadJson(){ //lets fake a json load method
	ofLog() << "trying to load json...";
	bool OK = (ofRandom(1) < 0.1); //fail 90% of the time
	return OK;
}


void ofApp::update(){

	switch (state.getState()) {

		case LOADING_ASSETS:
			if (state.getElapsedTime() > 2.0){ //lets pretend we load stuff for 2 seconds
				state.setState(LOADING_JSON);
			}
			break;

		case LOADING_JSON:

			if(state.isReadyToProceed()){
				if( !state.ranOutOfErrorRetries() ){

					bool OK = loadJson(); // YOUR STATE CODE HERE! <<<<<<<<<<<<<<<<<<<<<<<<<

					if(OK){
						ofLog() << "json loaded ok!";
						state.setState(STARTUP_TESTS);
					}else{
						state.setError("failed to load!", 3.0/*sec*/, 20/*retry max*/); //report an error, retry!
						ofLog() << "json failed to load! (" << state.getNumTimesRetried() << ")";
					}
				}else{
					ofLog() << "json failed to load too many times! Giving Up!";
					state.setState(JSON_FAILED);
				}
			}
			break;

		case JSON_FAILED:
			if (state.getElapsedTime() > 2.0){ //hold the error screen for a while and quit
				ofLog() << "cant load json, exiting!";
				ofExit();
			}
			break;

		case STARTUP_TESTS:
			if (state.getElapsedTime() > 2.0){
				state.setState(RUNNING);
			}
			break;

		case RUNNING:
			break;
	}
}


void ofApp::draw(){
	ofDrawBitmapString(state.getStatusMessage(), 20, 20);
}

