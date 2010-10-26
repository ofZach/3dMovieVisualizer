/*
 *  movementScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "movementScene.h"
#include "testApp.h"



void movementScene::setup(){
		
	
	lastFrameNum = -1;
	
	lastFrame.allocate(176, 144);
	currFrame.allocate(176, 144);
	
	flow.allocate(176, 144);
	
	
	dofShader.setup("shaders/DOFCloud");
	
	
	panel.setup("control", 0,150,300,700);
	panel.addPanel("3d control", 1, false);
	panel.setWhichPanel("3d control");
	
	panel.addSlider("max brightness", "maxBrightness", 0, 0, 255, true);
	panel.addSlider("rgb brightness", "rgbBrightness", 3, 0, 10, false);
	panel.addSlider("point brightness", "point_brightness", 10, 0, 20.0, false);
	panel.addSlider("dof focus offset", "focus_offset", 0, -4000, 4000, false);
	panel.addSlider("aberration", "aberration", 0.021, 0.005, 0.2, false);
	panel.addSlider("aperture", "aperture", 0.025, 0.001, 0.2, false);
	panel.addSlider("max point size", "maxPointSize", 21.3, 5, 40, false);
	
	
	
	panel.addSlider("vel threshold", "velthresh",2,0,100, false);
	
	panel.addSlider("random threshold", "ranthresh",0.4, 0,1, false);

	
	panel.addToggle("vel creates", "velcreates", false);
	
	panel.addToggle("sphere", "spherize", false);
	
	
	panel.loadSettings("movementSceneSettings.xml");
	
	
	
	
}

void movementScene::update(){
	
	panel.update();
	
	testApp * app = (testApp * ) ofGetAppPtr();
	
	int currLastFrame = app->processor.lastFrame;
	
	
	
	point3d * points = pointSet.points;
	
	if (currLastFrame != lastFrameNum){
		
		currFrame = app->processor.depthCv;
		
		unsigned char * pix = currFrame.getPixels();
		for (int i = 0; i < 176*144; i++){
			if (pix[i] > 230) pix[i] = 230;
		}
		currFrame.setFromPixels(pix, 176, 144);
		currFrame.blur(19);
		currFrame.blur(19);
		currFrame.blur(19);
		flow.calc(currFrame, lastFrame, 5);
		lastFrame = currFrame;
		lastFrameNum = currLastFrame;
		
		
		if (panel.getValueB("velcreates"))
		for(int i = 0; i < 144; i+= 1) {
			bool bInRun = false;
			for(int j = 0; j < 176; j++) {
				int n = (j  + i * 176);
				if (points[n].bVisibleThisFrame == true){
					
					//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
					ofSetColor(255,255,255,100);
					ofPoint flowAmt = flow.flowAtPoint(i,j);
					ofxVec3f flowPt;
					flowPt.set(flowAmt.x, flowAmt.y, 0);
					flowPt *= 4;
					if (flowPt.length() > panel.getValueF("velthresh") && ofRandom(0,1) >panel.getValueF("ranthresh") && panel.getValueB("spherize") == false){
						particle temp;
						temp.pos.set(points[n].pos.x - pointSet.midPointSmoothed.x , points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z  - pointSet.midPointSmoothed.z);
						
						
						float z = (points[n].pos.z  - pointSet.midPointSmoothed.z);
						float diff = z - 100;
						float pct = diff / 100.0f;
						if (pct < -1) pct = -1;
						if (pct > 1) pct = 1;
						
						temp.vel.set(flowPt.x*2, -flowPt.y*2,0);
						
						temp.color.set(points[n].color.x*255, points[n].color.y*255, points[n].color.z*255);
						particles.push_back(temp);
					} else {
						flowPt.set(0,0, 0);	
					}
				}
			}
		}
		
		
	}
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
		particles[i].addDampingForce();
		
		
		if (panel.getValueB("spherize") == true){
		ofxVec3f posp;
		posp.set(100,100,100);
		particles[i].addRepulsionForce(posp, 500, 10.1);
		particles[i].addAttractionForce(posp, 5000, 2.8);
		}
		particles[i].update();
	}
	
	//if (particles.size() > 34000){
//		particles.erase(particles.begin(), particles.begin() + (particles.size() - 34000));
//	}
	
	  
		
	std::vector<particle >::iterator iter = particles.begin();
	while (iter != particles.end())
	{
		if ( (fabs((*iter).vel.x) + fabs((*iter).vel.y)) <= 0.1)
		{
			
			iter = particles.erase(iter);
		}
		else
		{
			++iter;
		}
	}	
		
		
	
}

void movementScene::draw(){
	
	
	point3d * points = pointSet.points;
	
	ofEnableAlphaBlending();
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	
	// super helpful: http://pmviewer.sourceforge.net/method.php
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	
		
	
	
	glPointSize(1);
	
	glPushMatrix();
	
	glTranslatef(ofGetWidth()/2, ofGetHeight()/2,-2000);
	glRotatef(control.rotateY,-1,0,0);
	glRotatef(control.rotateX,0,1,0);
	
	
	//--------
	//
	
	
	dofShader.begin();
	
	float rgbBrightness = panel.getValueF("rgbBrightness");
	
	float pointBrightness = panel.getValueF("point_brightness");
	float aberration		= panel.getValueF("aberration");
	float aperture		= panel.getValueF("aperture");
	
	dofShader.setUniform("focusDistance", panel.getValueF("focus_offset"));
	dofShader.setUniform("aperture", aperture);
	dofShader.setUniform("pointBrightness", pointBrightness);
	dofShader.setUniform("rgbBrightness", rgbBrightness);
	dofShader.setUniform("maxPointSize", panel.getValueF("maxPointSize"));
	
	
	
	
	glBegin(GL_POINTS);
	//glPointSize(1);
	float v[4];
	
	if (panel.getValueB("spherize") == false){

		for(int i = 0; i < 144; i+= 1) {
			for(int j = 0; j < 176; j+=1) {
				int n = (j  + i * 176);
				if (points[n].bVisibleThisFrame == true){
					ofxVec3f pt;
					pt.set(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
					
					
					v[0] = points[n].color.x;
					v[1] = points[n].color.y;
					v[2] = points[n].color.z;
					v[3] = 0.3;
					
					//ofSetColor(points[n].color.x*255.0,points[n].color.y*255.0,points[n].color.z*255.0, 160);
					glColor4fv(v);
					glVertex3f(pt.x, pt.y, pt.z);
				}
			}
		}
	}
	glEnd();
	
	
		
	//glPointSize(4);
	
	glBegin(GL_POINTS);
	for (int i = 0; i < particles.size(); i++){
		
		
		float len = particles[i].vel.length();
		if (len > 4) len = 4;
		float pct = len / 4.0f;
		if (panel.getValueB("spherize") == false){
		ofSetColor(particles[i].color.x,particles[i].color.y,particles[i].color.z, 60*pct);
		} else {
			ofSetColor(particles[i].color.x,particles[i].color.y,particles[i].color.z, 20);
		}
		glVertex3f(particles[i].pos.x, particles[i].pos.y , particles[i].pos.z);
	}
	glEnd();
	
	ofSetColor(255,255,255);
	
	/*for(int i = 0; i < 144; i+= 1) {
		bool bInRun = false;
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			if (points[n].bVisibleThisFrame == true){
				
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				ofSetColor(255,255,255,100);
				
				ofPoint flowAmt = flow.flowAtPoint(i,j);
				
				
				ofxVec3f pt;
				pt.set(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
				
				ofxVec3f flowPt;
				flowPt.set(flowAmt.x, flowAmt.y, 0);
				
				flowPt *= 4;
				if (flowPt.length() > (ofGetAppPtr()->mouseX)){
					
				} else {
					flowPt.set(0,0, 0);	
				}
				//flowPt.limit(100);
				
				
				//float pct = flowPt.length() / 50.0f;
//				flowPt.normalize();
//				flowPt *= 50 * powf(pct, 1.3);
//				if (flowPt.length() > 25) flowPt *= 5;
//				
				
				glBegin(GL_LINES);
				glVertex3f(pt.x, pt.y, pt.z);
				glVertex3f(pt.x + flowPt.x, pt.y + flowPt.y, pt.z);
				glEnd();
				
				//glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
				
			
			
			} else {
				
			}
		}
		
	}*/
	
	
	dofShader.end();
	
	//--------
	
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,50);
	if (control.bDrawBox) ofxLineBox(pointSet.midpt.x - pointSet.midPointSmoothed.x, pointSet.midpt.y - pointSet.midPointSmoothed.y, pointSet.midpt.z- pointSet.midPointSmoothed.z, pointSet.top.x-pointSet.bottom.x, pointSet.top.y-pointSet.bottom.y, pointSet.top.z -pointSet.bottom.z);
	
	glPopMatrix();
	
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);
	
	
	panel.draw();
	
	//flow.draw(176*3, 144*3);
	
}