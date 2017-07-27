#include "ofApp.h"


void ofApp::setup(){

	ofBackground(22);
	ofSetFrameRate(60);

	//our content manaer
	contentManager.setup();

	dsm.setup("fonts/VeraMono-Bold.ttf", "", ofColor::black, ofColor::ivory);

	//listen to state machine changes
	ofAddListener(dsm.eventStateChanged, this, &ofApp::stateChanged);
	ofAddListener(dsm.eventStateError, this, &ofApp::stateError);

	//init state names
	dsm.SET_NAME_AND_COLOR_FOR_STATE(LOADING_CONTENT, ofColor::green, ofColor::gray);
	dsm.SET_NAME_AND_COLOR_FOR_STATE(LOADING_CONTENT_FAILED, ofColor::red, ofColor::gray);
	dsm.SET_NAME_AND_COLOR_FOR_STATE(RUNNING, ofColor::white, ofColor::gray);

	//set initial state;
	dsm.setState(LOADING_CONTENT);
}


void ofApp::stateChanged(ofxStateMachine<AppState>::StateChangedEventArgs& change){

	ofLog() << "State Changed from " << dsm.getNameForState(change.oldState) << " to "
	<< dsm.getNameForState(change.newState);

	switch(change.newState){

		case LOADING_CONTENT:
			contentManager.fetchContent();
			break;

		case LOADING_CONTENT_FAILED:
			break;

		case RUNNING:
			break;
	}
}


void ofApp::stateError(ofxStateMachine<AppState>::ErrorStateEventArgs& error){
	ofLog() << "Error '" << error.errorMsg << "' during state '" << dsm.getNameForState(error.state) << "'";
}


void ofApp::update(){

	switch (dsm.getState()) {

		case LOADING_CONTENT:

			contentManager.update();
			
			dsm.updateState( contentManager.getPercentDone(), "doing things...");

			if(dsm.isReadyToProceed() && !contentManager.isBusy()){ //slow down the state machine to handle error / retry

				if( dsm.hasError() && dsm.ranOutOfErrorRetries()){ //give up!
					ofLog() << "json failed to load too many times! Giving Up!";
					dsm.setState(LOADING_CONTENT_FAILED);
					break;
				}else{
					if(contentManager.isContentReady()){ //see if we are done (optional)
						ofLog() << "json loaded ok!";
						dsm.setState(RUNNING);
						break;
					}
				}

				if(contentManager.foundError()){
					dsm.setError("failed to load!", 3.0/*sec*/, 5/*retry max*/); //report an error, retry!
					ofLog() << "json failed to load! (" << dsm.getNumTimesRetried() << ")";
					dsm.setState(LOADING_CONTENT, false); //note "false" << do not clear errors (to keep track of # of retries)
				}

			}
			break;

		case LOADING_CONTENT_FAILED:
			dsm.updateState( dsm.getElapsedTimeInCurrentState() / jsonFailedTime, "doing things...");
			if (dsm.getElapsedTimeInCurrentState() > jsonFailedTime){ //hold the error screen for a while and quit
				ofLog() << "cant load json, exiting!";
				ofExit();
			}
			break;


		case RUNNING:
			dsm.updateState( dsm.getElapsedTimeInCurrentState() / jsonFailedTime, "doing things...");
			break;
	}
}


void ofApp::draw(){
	dsm.draw();
}

