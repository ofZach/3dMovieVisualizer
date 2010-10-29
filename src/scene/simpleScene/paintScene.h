/*
 *  simpleScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "ofx3DUtils.h"

#include "ofxFBOTexture.h"


class paintScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	
	
	ofxFBOTexture fbo[10];

	ofPoint randomColors[10];
	

	
};