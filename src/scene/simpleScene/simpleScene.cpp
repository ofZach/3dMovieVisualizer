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
	
	
	point3d * points = pointSet.points;
	
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
	
	int w = pointSet.nPointsW;
	int h = pointSet.nPointsH;
	
	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			int n = (i  + j * w);
			if (points[n].bVisibleThisFrame == true){
				ofSetColor(255,255,255,100);
				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
	 
			}
		}
	}
	glEnd();
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,50);
	ofxLineBox(pointSet.midpt.x - pointSet.midPointSmoothed.x, pointSet.midpt.y - pointSet.midPointSmoothed.y, pointSet.midpt.z- pointSet.midPointSmoothed.z, pointSet.top.x-pointSet.bottom.x, pointSet.top.y-pointSet.bottom.y, pointSet.top.z -pointSet.bottom.z);
	
	glPopMatrix();
	
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

}