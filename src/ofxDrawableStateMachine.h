//
//  ofxDrawableStateMachine.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 23/03/15.
//
//

#ifndef BaseApp_ofxDrawableStateMachine_h
#define BaseApp_ofxDrawableStateMachine_h


#include "ofMain.h"
#include "ofxStateMachine.h"
#include "ofxStateMachine.cpp" //NOTE THE INCLUSION OF .CPP!! Bc of the templated code
//http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

#include "LoadingScreen.h"

#define SET_NAME_AND_COLOR_FOR_STATE( STATE, BAR_COLOR, BAR_BG_COLOR ) \
setNameAndBarColorForState(STATE, #STATE, BAR_COLOR, BAR_BG_COLOR)

template <class T>
class ofxDrawableStateMachine: public ofxStateMachine<T>{

public:

	void setup(string fontPath, string svgLogoPath,
			   ofColor screenBgColor, ofColor textColor){
		screen.setup(fontPath, svgLogoPath, screenBgColor, textColor);
		
	}

	void setNameAndBarColorForState(T state, string name, ofColor barColor, ofColor barBGColor){
		ofxStateMachine<T>::setNameForState( state, name);
		stateColors[state] = barColor;
		stateBgColors[state] = barBGColor;
	}

	void updateState(float progress/*[0..1]*/, string status){
		string stateStatus = ofxStateMachine<T>::getStatusMessage();
		screen.update(progress, status + "\n\n" + stateStatus , ofGetLastFrameTime());
		screen.updateScreenName(ofxStateMachine<T>::getNameAndQuickStatusForState(ofxStateMachine<T>::state));
	}

	void draw(ofRectangle r = ofRectangle(0,0, ofGetWidth(), ofGetHeight())){
		ofPushStyle();
		ofRectangle area = screen.draw(r);
		ofNotifyEvent(eventDraw, area, this);
		ofPopStyle();
	}

	void setStateProgress(float progress ); ///progress [0..1]

	void setProgressBarExtraInfo(string newName){
		screen.setExtraBarInfo(newName);
	}

	void setState(T newState, bool clearErrors = true){

		ofxStateMachine<T>::setState(newState, clearErrors);

		ofColor barFillColor = ofColor::green;
		typename map<T, ofColor>::iterator it = stateColors.find(newState);
		if(it != stateColors.end()){
			barFillColor = it->second;
		}

		ofColor barBgColor = ofColor::green * 0.5;
		it = stateBgColors.find(newState);
		if(it != stateBgColors.end()){
			barBgColor = it->second;
		}

		screen.newScreen(ofxStateMachine<T>::getCurrentStateName(), //screen name
						 barBgColor, //prog bar bg color
						 barFillColor //prog bar fill color
						 );
	}

	ofEvent<ofRectangle> eventDraw;

private:

	LoadingScreen screen;
	map<T, ofColor> stateColors;
	map<T, ofColor> stateBgColors;

};

#endif
