//
//  LoadingScreen.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 04/04/14.
//
//

#ifndef __BaseApp__LoadingScreen__
#define __BaseApp__LoadingScreen__


#include "ofMain.h"
#include "ProgressBar.h"
#include "ofxFontStash.h"
#include "ofxSvg.h"


class LoadingScreen{

public:

	LoadingScreen();
	void setup(string fontName, string logoSvgPath, ofColor bgColor, ofColor status);

	//for each new "stage", tell me your stage name, colors
	void newScreen(	string screenName,
					ofColor progressBarBgColor,
					ofColor progressBarColor
			   		);

	void setExtraBarInfo(string barInfo);

	void updateScreenName(string s);

	//then slowly increase that number to animate the progressbar
	void update(float currentValue, string status, float dt);

	//returns area above progress bar, so you can custom draw if you want to.
	ofRectangle draw(ofRectangle r);

private:

	ProgressBar				progress;

	ofxFontStash* 			font;
	ofxSVG					lpLogoSvg;



	string statusString;
	string currentScreenName;
	string extraBarInfo;

	ofColor bgColor;
	ofColor statusColor;
};

#endif /* defined(__BaseApp__LoadingScreen__) */
