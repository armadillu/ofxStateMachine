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

#ifdef LOGO_SUPPORT
#include "ofxSvg.h"
#endif


class LoadingScreen{

public:

	LoadingScreen();
	void setup(std::string fontName, std::string logoSvgPath, ofColor bgColor, ofColor status);

	//for each new "stage", tell me your stage name, colors
	void newScreen(	std::string screenName,
					ofColor progressBarBgColor,
					ofColor progressBarColor
			   		);

	void setExtraBarInfo(std::string barInfo);

	void updateScreenName(std::string s);

	//then slowly increase that number to animate the progressbar
	void update(float currentValue, std::string status, float dt);

	//returns area above progress bar, so you can custom draw if you want to.
	ofRectangle getDrawableArea(){return drawArea;}
	void draw(ofRectangle r);

private:

	ProgressBar				progress;

	ofTrueTypeFont			font2;
	#ifdef LOGO_SUPPORT
	ofxSVG					lpLogoSvg;
	#endif

	std::string statusString;
	std::string currentScreenName;
	std::string extraBarInfo;

	ofColor bgColor;
	ofColor statusColor;
	ofRectangle drawArea;
};

#endif /* defined(__BaseApp__LoadingScreen__) */
