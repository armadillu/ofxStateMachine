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
	ofDisableArbTex();
	indeterminateFbo.allocate(INDETERMINATE_BAR_SIZE, INDETERMINATE_BAR_SIZE, GL_RGBA, 4);
	ofEnableArbTex();
}

void ProgressBar::setup(float startingValue, float maxValue_, float animFilter){
	maxValue = maxValue_;
	setValue(startingValue);
	filter = animFilter;
	indeterminate = false;
}

void ProgressBar::setProgressIsIndeterminate(bool i){

	if(i && i != indeterminate){ //update fbo for indeterminate colors
		ofPushStyle();
		indeterminateFbo.begin();
		ofClear(bgColor);
		ofSetColor(fgColor, 200);
		float w = INDETERMINATE_BAR_SIZE / 8;
		for(int i = -1; i < 2; i++){
			ofBeginShape();
			int x = i * INDETERMINATE_BAR_SIZE / 2;
			int x2 = (i+1) * INDETERMINATE_BAR_SIZE / 2;
			ofVertex(x2 - w, 0);
			ofVertex(x2 + w, 0);
			ofVertex(x + w, INDETERMINATE_BAR_SIZE);
			ofVertex(x - w, INDETERMINATE_BAR_SIZE);
			ofEndShape();
		}

		indeterminateFbo.end();
		indeterminateFbo.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);

		ofPopStyle();
	}
	indeterminate = i;
}

void ProgressBar::update(float dt){
	float inc = (targetVal - val ) * filter;
	val += inc;
}

void ProgressBar::setAnimatedValue(float v){
	v = ofClamp(v, 0.0f, maxValue);
	targetVal = v;
	indeterminate = false;
}


void ProgressBar::setValue(float v){
	v = ofClamp(v, 0.0f, maxValue);
	val = targetVal = v;
	indeterminate = false;
}
 
void ProgressBar::draw(const ofRectangle & r, float a){
	draw(r.x, r.y, r.width, r.height, a);
}

void ProgressBar::draw(float x, float y, float width, float height, float a){

	float percent = ofClamp( val / maxValue, 0, 1);

	if(percent != percent){percent = 1.0f;} //NAN
	ofPushStyle();
	if(!indeterminate){
		ofSetColor(ofColor(bgColor, 255 * a));
		ofDrawRectangle( x + width * percent, y, width * (1.0f-percent), height);
		ofSetColor(ofColor(fgColor, 255 * a));
		ofDrawRectangle( x, y, width * percent, height);
	}else{
		ofSetColor(255);
		indeterminateFbo.getTextureReference().bind();
		float scale = 1.0;

		ofMesh m;
		float indetOffset = - ofGetElapsedTimef();
		m.setMode(OF_PRIMITIVE_TRIANGLES);

		m.addVertex(ofVec2f(x,y));
		m.addTexCoord(ofVec2f(indetOffset, 0));

		m.addVertex(ofVec2f(x + width, y));
		m.addTexCoord(ofVec2f(indetOffset + scale * width / INDETERMINATE_BAR_SIZE , 0));

		m.addVertex(ofVec2f(x + width, y + height));
		m.addTexCoord(ofVec2f(indetOffset + scale * width / INDETERMINATE_BAR_SIZE, scale * height / INDETERMINATE_BAR_SIZE));

		m.addVertex(ofVec2f(x, y + height));
		m.addTexCoord(ofVec2f(indetOffset, scale * height / INDETERMINATE_BAR_SIZE));

		m.addIndex(0); m.addIndex(1); m.addIndex(2);
		m.addIndex(0); m.addIndex(2); m.addIndex(3);
		m.draw();
		indeterminateFbo.getTextureReference().unbind();

	}
	ofPopStyle();
}

