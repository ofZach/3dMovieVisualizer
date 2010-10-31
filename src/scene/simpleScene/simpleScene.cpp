/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "simpleScene.h"


void simpleScene::setup(){
		
}

void simpleScene::update(){
	
}

void simpleScene::draw(){
	
	
	
	for (int k = 0; k < pointSetSet.nSets; k++){
		
		point3d * points = pointSetSet.pointSets[k].points;
		
		ofEnableAlphaBlending();
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH);
		glEnable(GL_POINT_SMOOTH);
		glPointSize(3);
		
		glPushMatrix();
		
		glTranslatef(ofGetWidth()/2, ofGetHeight()/2,-2000);
		glRotatef(control.rotateY,-1,0,0);
		glRotatef(control.rotateX,0,1,0);
		glBegin(GL_POINTS);
		for(int i = 0; i < 176; i++) {
			for(int j = 0; j < 144; j++) {
				int n = (i  + j * 176);
				if (points[n].bVisibleThisFrame == true){
					ofSetColor(255,255,255,100);
					glVertex3f(points[n].pos.x - pointSetSet.midPointSmoothed.x, points[n].pos.y - pointSetSet.midPointSmoothed.y, points[n].pos.z - pointSetSet.midPointSmoothed.z);
		 
				}
			}
		}
		glEnd();
		ofEnableAlphaBlending();
		ofSetColor(255,255,255,50);
		ofxLineBox(pointSetSet.midpt.x - pointSetSet.midPointSmoothed.x, pointSetSet.midpt.y - pointSetSet.midPointSmoothed.y, pointSetSet.midpt.z- pointSetSet.midPointSmoothed.z, pointSetSet.top.x-pointSetSet.bottom.x, pointSetSet.top.y-pointSetSet.bottom.y, pointSetSet.top.z -pointSetSet.bottom.z);
		
		glPopMatrix();
		
		glDisable(GL_DEPTH);
		glDisable(GL_DEPTH_TEST);
	}

}