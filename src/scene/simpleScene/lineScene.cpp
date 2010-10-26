/*
 *  lineScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "lineScene.h"


void lineScene::setup(){
	
}

void lineScene::update(){
	
}

void lineScene::draw(){
	
	
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
	
	
	//--------
	
	/*for(int i = 0; i < 144; i+= 1) {
		bool bInRun = false;
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			if (points[n].bVisibleThisFrame == true){
				if (bInRun == false){
					bInRun = true;	
					glBegin(GL_LINE_STRIP);
				}
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				ofSetColor(255,255,255,100);
				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
			} else {
				if (bInRun == true){
					glEnd();	
					bInRun = false;
				}
			}
		}
		if (bInRun == true){
			glEnd();	
			bInRun = false;
		}
	}*/
	
	
	int step = 2; //(int)ofMap(sin(ofGetElapsedTimef()*20), -1,1, 1,8);
	for(int i = 0; i < 144; i+= step) {
		bool bInRun = false;
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			
			//float pctz = ( points[n].pos.z - pointSet.bottom.z) / (pointSet.top.z - pointSet.bottom.z);
			//pctz = powf(pctz, 3.0);
			//int clr = ofMap(pctz, 0,1,50, 255);
			
			if (points[n].bVisibleThisFrame == true){
				ofSetColor(200,200,200,100);
				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
				
			} else {
				ofSetColor(200,200,200,100);
				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y,  points[n].pos.z - pointSet.midPointSmoothed.z);
				
				
			}
			
		}
		glEnd();	
		
	}
	
	
	
	
	//--------
	
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,50);
	if (control.bDrawBox) ofxLineBox(pointSet.midpt.x - pointSet.midPointSmoothed.x, pointSet.midpt.y - pointSet.midPointSmoothed.y, pointSet.midpt.z- pointSet.midPointSmoothed.z, pointSet.top.x-pointSet.bottom.x, pointSet.top.y-pointSet.bottom.y, pointSet.top.z -pointSet.bottom.z);
	
	glPopMatrix();
	
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);
	
}