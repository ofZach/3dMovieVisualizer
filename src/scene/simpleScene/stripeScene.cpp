/*
 *  stripeScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "stripeScene.h"


void stripeScene::setup(){
	
		
}

void stripeScene::update(){
	
}

void stripeScene::draw(){
	
	
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
	
	for(int i = 0; i < 144-1; i+= 1) {
		for(int j = 0; j < 176-1; j++) {
			
			int n0 = (j  + i * 176);
			int n1 = ((j+1) + i * 176);
			int n2 = ((j+1)  + (i+1) * 176);
			int n3 = (j  + (i+1) * 176);
			
			
			if (points[n0].bVisibleThisFrame == true &&
				points[n1].bVisibleThisFrame == true &&
				points[n2].bVisibleThisFrame == true &&
				points[n3].bVisibleThisFrame == true
				){
				
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				ofSetColor(255,255,255,100);
				
				ofxVec3f pta = ofxVec3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				ofxVec3f ptb = ofxVec3f(points[n1].pos.x - pointSet.midPointSmoothed.x, points[n1].pos.y - pointSet.midPointSmoothed.y, points[n1].pos.z - pointSet.midPointSmoothed.z);
				ofxVec3f ptc = ofxVec3f(points[n2].pos.x - pointSet.midPointSmoothed.x, points[n2].pos.y - pointSet.midPointSmoothed.y, points[n2].pos.z - pointSet.midPointSmoothed.z);
				ofxVec3f ptd = ofxVec3f(points[n3].pos.x - pointSet.midPointSmoothed.x, points[n3].pos.y - pointSet.midPointSmoothed.y, points[n3].pos.z - pointSet.midPointSmoothed.z);
				
				glBegin(GL_QUADS);
				
				glVertex3f(pta.x, pta.y, pta.z);
				glVertex3f(ptb.x, ptb.y, ptb.z);
				glVertex3f(ptc.x, ptc.y, ptc.z);
				glVertex3f(ptd.x, pta.y, ptd.z);
				
				
				glEnd();
				
				//glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
				
			
			}
		}
		
	}
	
	
	
	
	//--------
	
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,50);
	ofxLineBox(pointSet.midpt.x - pointSet.midPointSmoothed.x, pointSet.midpt.y - pointSet.midPointSmoothed.y, pointSet.midpt.z- pointSet.midPointSmoothed.z, pointSet.top.x-pointSet.bottom.x, pointSet.top.y-pointSet.bottom.y, pointSet.top.z -pointSet.bottom.z);
	
	glPopMatrix();
	
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

}


