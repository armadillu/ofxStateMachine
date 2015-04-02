/*
 *  ProgressBar.h
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 20/01/11.
 *  Copyright 2011 uri.cat. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#define INDETERMINATE_BAR_SIZE 128.0f

class ProgressBar{

public:
	
	ProgressBar();
	void setup(float startingValue, float targetValue, float animationFilter = 0.2);
	void update(float dt);
	void draw(float x, float y, float width, float height, float a = 1.0f);
	void draw(const ofRectangle & r, float a = 1.0f);
	
	void setValue(float v);
	void setAnimatedValue(float v);
	void setProgressIsIndeterminate(bool i);
	bool getBarProgressIsIndeterminate(){return indeterminate;}


	void setFGColor( ofColor c){ fgColor = c; }
	void setBGColor( ofColor c){ bgColor = c; }

	float getCurrentValue(){return val;}
private:

	//animated bar
	float val;
	float targetVal;
	float filter;

	bool indeterminate;
	float maxValue;

	ofColor fgColor;
	ofColor bgColor;
	ofFbo indeterminateFbo;
};

