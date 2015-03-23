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


class ProgressBar{

	public:
	
	ProgressBar();
	void setup(float startingValue, float targetValue, float animationFilter = 0.1);
	void update(float dt);
	void draw(float x, float y, float width, float height, float a = 1.0f);
	void draw(const ofRectangle & r, float a = 1.0f);
	
	void setValue(float v);
	void setAnimatedValue(float v);

	void setFGColor( ofColor c){ fgColor = c; }
	void setBGColor( ofColor c){ bgColor = c; }

	private:

	//animated bar
	float val;
	float targetVal;
	float filter;

	float maxValue;

	ofColor fgColor;
	ofColor bgColor;
};

