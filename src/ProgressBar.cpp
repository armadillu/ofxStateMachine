/*
 *  ProgressBar.cpp
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 20/01/11.
 *  Copyright 2011 uri.cat. All rights reserved.
 *
 */

#include "ProgressBar.h"

ProgressBar::ProgressBar(){
	val = 0;
	targetVal = 0;
	bgColor = ofColor::green;
	fgColor = ofColor::white;
}

void ProgressBar::setup(float startingValue, float maxValue_, float animFilter){
	maxValue = maxValue_;
	setValue(startingValue);
	filter = animFilter;
}

void ProgressBar::update(float dt){
	float inc = (targetVal - val ) * filter;
	val += inc;
}

void ProgressBar::setAnimatedValue(float v){
	v = ofClamp(v, 0.0f, maxValue);
	targetVal = v;
}


void ProgressBar::setValue(float v){
	v = ofClamp(v, 0.0f, maxValue);
	val = targetVal = v;
}
 
void ProgressBar::draw(const ofRectangle & r, float a){
	draw(r.x, r.y, r.width, r.height, a);
}

void ProgressBar::draw(float x, float y, float width, float height, float a){

	float percent = ofClamp( val / maxValue, 0, 1);

	if(percent != percent){percent = 1.0f;} //NAN
	ofPushStyle();
	ofSetColor(ofColor(bgColor, 255 * a));
	ofDrawRectangle( x + width * percent, y, width * (1.0f-percent), height);
	ofSetColor(ofColor(fgColor, 255 * a));
	ofDrawRectangle( x, y, width * percent, height);
	ofPopStyle();
}

