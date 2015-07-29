//
//  ofxStateMachine.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 30/07/14.
//
//

#include "ofxStateMachine.h"


template <class T>
ofxStateMachine<T>::ofxStateMachine(){
	stateTime = waitTime = 0.0f;
	firstState = true;
	clearErrorStatus();
	ofAddListener(ofEvents().update, this, &ofxStateMachine::update);
}


template <class T>
ofxStateMachine<T>::~ofxStateMachine(){
	ofRemoveListener(ofEvents().update, this, &ofxStateMachine::update);
}


template <class T>
void ofxStateMachine<T>::setNameForState(T state_, string name){
	stateNames[state_] = name;
}


template <class T>
void ofxStateMachine<T>::setState(T newState, bool clearErrors){

	if(clearErrors){
		stateTime = waitTime = 0.0f;
		clearErrorStatus();
	}

	if(firstState){
		prevState = state = newState;
		firstState = false;
	}
	prevState = state;
	state = newState;
	StateChangedEventArgs eventArgs = {prevState, state};

	ofNotifyEvent(eventStateChanged, eventArgs, this);
}


template <class T>
T ofxStateMachine<T>::getState(){
	return state;
}


template <class T>
string ofxStateMachine<T>::getNameForState(T state_){
	typename map<T, string>::iterator it = stateNames.find(state_);
	if (it != stateNames.end()){
		return it->second;
	}
	return "state " + ofToString((int)state_) + " (no name given)";
}


template <class T>
string ofxStateMachine<T>::getNameAndQuickStatusForState(T state_){
	string r = getNameForState(state_);
	if(error){
		r += " ERROR! (" + ofToString(numRetries) + "/" + ofToString(maxRetries) + ")";
	}
	return r;
}


template <class T>
string ofxStateMachine<T>::getCurrentStateName(){
	return stateNames[state];
}


template <class T>
bool ofxStateMachine<T>::hasError(){ return error; }


template <class T>
void ofxStateMachine<T>::update(ofEventArgs & args){
	float dt = ofGetLastFrameTime();
	stateTime += dt;
	waitTime -= dt;
	if (waitTime < 0.0f) waitTime = 0.0f;
}


template <class T>
bool ofxStateMachine<T>::isReadyToProceed(){
	return waitTime <= 0.0f;
};


template <class T>
void ofxStateMachine<T>::setError(string msg, float retryAgainInSeconds, int maxRetries_){

	error = true;
	errorMsg = msg;
	numRetries++;
	waitTime = retryAgainInSeconds;
	maxRetries = maxRetries_;
	ofLogError("ofxStateMachine") << "Error '" << msg << "' while in state '" << getCurrentStateName() << "' "
	<< "(" << numRetries<< "/" << maxRetries << ")";
	ErrorStateEventArgs errArgs = {state, msg, numRetries};
	ofNotifyEvent(eventStateError, errArgs, this);

};


template <class T>
string ofxStateMachine<T>::getStatusMessage(){
	string msg = "State: " + getNameForState(state);
	msg += "\nTime in State: " + ofToString(stateTime);
	if(error){
		msg += "\nError: " + errorMsg;
		msg += "\nRetrying in: " + ofToString(waitTime, 1) + " seconds";
		msg += "\nRetries left: " + ofToString(maxRetries - numRetries + 1);
	}
	return msg;
}


template <class T>
void ofxStateMachine<T>::clearErrorStatus(){
	numRetries = 0;
	maxRetries = 0;
	error = false;
	errorMsg = "";
}

template <class T>
float ofxStateMachine<T>::getElapsedTimeInCurrentState(){ return stateTime;}


template <class T>
int ofxStateMachine<T>::getNumTimesRetried(){
	return numRetries;
}


template <class T>
int ofxStateMachine<T>::getMaxRetryTimes(){
	return maxRetries;
}


template <class T>
bool ofxStateMachine<T>::ranOutOfErrorRetries(){
	return error && (numRetries >= maxRetries);
}


