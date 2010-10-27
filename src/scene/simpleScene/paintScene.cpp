/*
 *  paintScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "paintScene.h"


void paintScene::setup(){
	
	for (int i = 0; i < 10; i++){
		fbo[i].allocate(640,480, GL_RGBA, 4);
		randomColors[i].set(ofRandom(127,255), ofRandom(127,255),ofRandom(127,255));
	}
}

void paintScene::update(){
	
	
	// ok let's do it! 
	
	point3d * points = pointSet.points;
	
	
	//(i / 176.0 * ofGetWidth() - ofGetWidth()/2.0)*3
	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	
	//
	for (int k = 0; k < 10; k++){
		fbo[k].begin();
		//fbo[k].clear(0,0,0,0.01);
		ofSetColor(0,0,0,200);
		ofRect(0,0,640,480);
		glBegin(GL_POINTS);
		for(int i = 0; i < 176; i++) {
			for(int j = 0; j < 144; j++) {
				
				float lowZ = pointSet.bottom.z + (k) * ((pointSet.top.z-pointSet.bottom.z)/9.0f);
				float highZ = pointSet.bottom.z + (k+1) * ((pointSet.top.z-pointSet.bottom.z)/9.0f);
				
				int n = (i  + j * 176);
				if (points[n].bVisibleThisFrame == true){
					
					
					
					if (points[n].pos.z >= lowZ &&  points[n].pos.z < highZ){
						ofSetColor(randomColors[k].x,randomColors[k].y,randomColors[k].z,120);
						
						
						float posInOrigx = ((points[n].pos.x/3)  + ofGetWidth()/2.0) / (float)(ofGetWidth()) * 640;
						float posInOrigy = ((points[n].pos.y/3)  + ofGetHeight()/2.0) / (float)(ofGetHeight()) * 480;
						
						glVertex2f(	posInOrigx, posInOrigy);
						//ofRect(posInOrigx, posInOrigy, 5,5);
						
					}
				}
			}
		}
		glEnd();
		
		
		fbo[k].end();
	}
	
	//glEnd();
	
	
}

void paintScene::draw(){
	
	
	//float scalef = (pointSet.top.y - pointSet.bottom.y) / 480.0f;
	//float offset = 
	
	
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
	
	
	
	float scalef = ((1.0/3.0)  + ofGetWidth()/2.0) / (float)(ofGetWidth());
	ofEnableAlphaBlending();
	
	
	
	glBlendFunc(GL_ONE, GL_ONE);
	

	ofSetColor(255,255,255,160);
	
	for (int i = 0; i < 10; i++){
		
		float z = pointSet.bottom.z + (i) * ((pointSet.top.z-pointSet.bottom.z)/9.0f);
		glPushMatrix();
		
		glTranslatef(-320  -  pointSet.midPointSmoothed.x,-240 -  pointSet.midPointSmoothed.y, z -  pointSet.midPointSmoothed.z);
		glScalef(2.66/scalef + i*0.01f, 2.66/scalef + i*0.01f, 1);
		fbo[i].draw(-320+60,-240+60);
		glPopMatrix();
	}
	
		
	
	
	ofEnableAlphaBlending();
	
	glBegin(GL_POINTS);
	for(int i = 0; i < 176; i++) {
		for(int j = 0; j < 144; j++) {
			int n = (i  + j * 176);
			if (points[n].bVisibleThisFrame == true){
				ofSetColor(255,255,255,30);
				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
				
			}
		}
	}
	glEnd();
	
	//--------
	
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,50);
	if (control.bDrawBox) ofxLineBox(pointSet.midpt.x - pointSet.midPointSmoothed.x, pointSet.midpt.y - pointSet.midPointSmoothed.y, pointSet.midpt.z- pointSet.midPointSmoothed.z, pointSet.top.x-pointSet.bottom.x, pointSet.top.y-pointSet.bottom.y, pointSet.top.z -pointSet.bottom.z);
	
	glPopMatrix();
	
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);
	
}