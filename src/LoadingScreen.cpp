//
//  LoadingScreen.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 04/04/14.
//
//

#include "LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(){
}


void LoadingScreen::setup(string fontName, string logoSvgPath, ofColor bgColor_, ofColor statusC){

	#ifdef LOGO_SUPPORT
	if(ofFile::doesFileExist(logoSvgPath)){
		lpLogoSvg.load(logoSvgPath);
		for(int i = 0; i < lpLogoSvg.getNumPath(); i++){
			lpLogoSvg.getPathAt(i).setFillColor(32);
		}
	}else{
		ofLogError("LoadingScreen") << "SVG LOGO file not found " << logoSvgPath;
	}
	#endif
	statusColor = statusC;
	bgColor = bgColor_;
	float fontSizeGuess = ofGetHeight() / 70.;
	font2.load(fontName, fontSizeGuess, true, false, false);
	font2.setLineHeight(fontSizeGuess * 1.33);
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


void LoadingScreen::draw(ofRectangle bounds){

	float avgSize = (bounds.width + bounds.height) * 0.5;

	ofSetColor(bgColor);
	ofDrawRectangle(bounds);
	float padding = avgSize / 40;

	//status info
	float fontSize = ofClamp(bounds.height / 50, 11, 50);
	float lineH = font2.getLineHeight();
	float usableDrawH = (bounds.height - 5 * padding); //above the progress bar + 1 extra padding
	float numLines = usableDrawH / lineH; //more or less how many lines can fit in this space?

	//as this could be lots of lines, lets remove the lines that would be offscreen so that we dont have to draw them
	vector<string> splitLines = ofSplitString(statusString, "\n");
	if (splitLines.size() > numLines){
		splitLines.erase(splitLines.begin(), splitLines.begin() + splitLines.size() - 1 - numLines);
	}

	string shortMsg;
	for(size_t i = 0; i < splitLines.size(); i++){
		shortMsg += splitLines[i] + "\n";
	}

	ofVec2f msgBox;
	ofRectangle bbox = font2.getStringBoundingBox(shortMsg, 0,0 );
	msgBox.y = bbox.height;
	msgBox.x = bbox.width;

	//offset to implement a cheap auto-scroll when status message is longer than the area we have available
	float offsetY = 0;

	if ( msgBox.y > usableDrawH ){
		offsetY = msgBox.y - usableDrawH;
	}
	ofSetColor(statusColor);
	ofPushMatrix();
		ofTranslate(padding + 1.0f, padding + 1.0f - offsetY);
		font2.drawString(shortMsg, 0, 0);
	ofPopMatrix();

	//progress bar
	float barH = avgSize / 40.;
	float barY = bounds.height - padding * 3;
	progress.draw( padding, barY, bounds.width - 2 * padding, barH);
	#ifdef LOGO_SUPPORT
	float svgW = lpLogoSvg.getWidth();
	float logoW = 2 * padding;//ofClamp(padding, 50, 150);
	float scale = logoW / svgW;
	#endif

	//progress bar font size bigger
	fontSize = barH * 0.6;
	string t = currentScreenName;
	ofStringReplace(t, "_", " ");
	string t2 = extraBarInfo;
	ofStringReplace(t2, "_", " ");

	string msg = t + " " + t2;
	ofRectangle r = font2.getStringBoundingBox(msg, 0, 0);
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
	float off = 0.5 * fabs(r.height - barH);
	font2.drawString(msg, bounds.width / 2 - r.width / 2, barY + barH - off - (r.height + r.y));

	//LP logo
	#ifdef LOGO_SUPPORT
	ofPushMatrix();
		ofTranslate(drawArea.width - padding - logoW, barY + barH + padding * 0.5 );
		ofScale(scale, scale);
		lpLogoSvg.draw();
	ofPopMatrix();
	#endif

	drawArea = ofRectangle(padding, 2 * padding, bounds.width - 2 * padding, barY - 3 * padding );
}


