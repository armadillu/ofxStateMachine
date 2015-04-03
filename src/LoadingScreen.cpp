//
//  LoadingScreen.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 04/04/14.
//
//

#include "LoadingScreen.h"

LoadingScreen::LoadingScreen(){
}


void LoadingScreen::setup(string fontName, string logoSvgPath, ofColor bgColor_, ofColor statusC){

	if(ofFile::doesFileExist(logoSvgPath)){
		lpLogoSvg.load(logoSvgPath);
		for(int i = 0; i < lpLogoSvg.getNumPath(); i++){
			lpLogoSvg.getPathAt(i).setFillColor(32);
		}
	}else{
		ofLogError("LoadingScreen") << "SVG LOGO file not found " << logoSvgPath;
	}
	statusColor = statusC;
	bgColor = bgColor_;
	font = new ofxFontStash();
	font->setup(fontName, 1.33);
}


void LoadingScreen::newScreen(string screenName,
							  ofColor progressBarBgC,
							  ofColor progressBarColor){

	currentScreenName = screenName;
	progress.setup(0, 1, 0.3);
	progress.setBGColor(progressBarBgC);
	progress.setFGColor(progressBarColor);
}


void LoadingScreen::setExtraBarInfo(string tempName){
	extraBarInfo = tempName;
}

void LoadingScreen::updateScreenName(string s){
	currentScreenName = s;
}

void LoadingScreen::update(float currentValue, string status, float dt){

	statusString = status;
	if(currentValue >= 0.0 && currentValue <= 1.0f){
		progress.setProgressIsIndeterminate(false);
		if (progress.getCurrentValue() > currentValue){ //if going backwards, jump!
			progress.setValue(currentValue);
		}else{ //else animate
			progress.setAnimatedValue(currentValue);
		}
	}else{ // indeterminate!
		progress.setProgressIsIndeterminate(true);
	}

	progress.update(dt);
}


ofRectangle LoadingScreen::draw(ofRectangle drawArea){

	if(!font){
		ofLogError("LoadingScreen") << "Font Not Set!";
		return;
	}

	ofSetColor(bgColor);
	ofRect(drawArea);
	float padding = drawArea.width/20;

	//status info
	float fontSize = ofClamp(drawArea.width / 70., 11, 80);
	ofSetColor(0, 200);
	font->drawMultiLine(statusString, fontSize, padding + 1.0f, padding + 1.0f);
	ofSetColor(statusColor);
	font->drawMultiLine(statusString, fontSize, padding, padding);


	//progress bar
	float barH = ofGetHeight() / 30.;
	float barY = ofGetHeight() * 0.80;
	progress.draw( padding, barY, drawArea.width - 2 * padding, barH);
	float svgW = lpLogoSvg.getWidth();
	float imgW = padding;//ofClamp(padding, 50, 150);
	float scale = imgW / svgW;

	//progress bar font size bigger
	fontSize *= 1.75;
	string msg = currentScreenName + " " + extraBarInfo;
	ofRectangle r = font->getBBox(msg, fontSize, 0, 0);
	switch(((int)(ofGetFrameNum() * 0.2))%9){
		case 0: msg += "...  ";break;
		case 1: msg += " ... ";break;
		case 2: msg += "  ...";break;
		case 3: msg += "   ..";break;
		case 4: msg += "    .";break;
		case 5: msg += "     ";break;
		case 7: msg += ".    ";break;
		case 8: msg += "..   ";break;
	}
	ofSetColor(255);
	font->draw(msg, fontSize, ofGetWidth()/2 - r.width / 2, barY - r.y -0.5f * (r.height -barH) );

	//LP logo
	ofPushMatrix();
		ofTranslate(ofGetWidth() - 2 * imgW, barY + barH * 2);
		ofScale(scale, scale);
		lpLogoSvg.draw();
	ofPopMatrix();

	ofRectangle areaAboveProgress = ofRectangle(padding, padding,
												drawArea.width - 2 * padding, barY - 2 * padding );
	return areaAboveProgress;
}


