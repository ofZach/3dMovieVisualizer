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
#include "ofxCvOpticalFlowLK.h"
#include "particle.h"
#include "ofxShader.h"
#include "ofxControlPanel.h"



class movementLineDOFScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	
	void mousePressed(int x, int y, int button){
		panel.mousePressed(x, y, button);
	}
	void mouseDragged(int x, int y, int button){
		panel.mouseDragged(x, y, button);
	}
	void mouseReleased(){
		panel.mouseReleased();
	}
	
	
	ofxCvGrayscaleImage lastFrame;
	ofxCvGrayscaleImage currFrame;
	
	int lastFrameNum;
	
	ofxCvOpticalFlowLK flow;
	
	vector <particle> particles;
	
	ofxShader dofShader;
	ofxShader dofLineShader;
	
	ofxControlPanel		panel;
	
};