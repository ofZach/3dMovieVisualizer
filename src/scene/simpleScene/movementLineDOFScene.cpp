/*
 *  movementLineDOFScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "movementLineDOFScene.h"
#include "testApp.h"



void movementLineDOFScene::setup(){
		
	
	lastFrameNum = -1;
	
	lastFrame.allocate(176, 144);
	currFrame.allocate(176, 144);
	
	flow.allocate(176, 144);
	
	
	dofShader.setup("shaders/DOFCloud");
	dofLineShader.setup("shaders/DOFCloudLine");
	
	panel.setup("control", 0,150,300,700);
	panel.addPanel("3d control", 1, false);
	panel.setWhichPanel("3d control");
	
	panel.addSlider("max brightness", "maxBrightness", 0, 0, 255, true);
	panel.addSlider("rgb brightness", "rgbBrightness", 3, 0, 10, false);
	panel.addSlider("point brightness", "point_brightness", 10, 0, 20.0, false);
	panel.addSlider("dof focus offset", "focus_offset", 0, -4000, 4000, false);
	panel.addSlider("aberration", "aberration", 0.021, 0.005, 0.2, false);
	panel.addSlider("aperture", "aperture", 0.025, 0.001, 0.2, false);
	panel.addSlider("max point size", "maxPointSize", 21.3, 5, 120, false);
	
	
	
	panel.addSlider("vel threshold", "velthresh",2,0,100, false);
	
	panel.addSlider("random threshold", "ranthresh",0.4, 0,1, false);

	
	panel.addToggle("vel creates", "velcreates", false);
	
	panel.addToggle("sphere", "spherize", false);
	
	
	panel.loadSettings("movementLineDOFSceneSettings.xml");
	
	
	
	
}

void movementLineDOFScene::update(){
	
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

void movementLineDOFScene::draw(){
	
	
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
	
	
	
	
	dofLineShader.begin();
	

	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);
	
	
	
	float rgbBrightness = panel.getValueF("rgbBrightness");
	float pointBrightness = panel.getValueF("point_brightness");
	float aberration		= panel.getValueF("aberration");
	float aperture		= panel.getValueF("aperture");
	dofLineShader.setUniform("focusDistance", panel.getValueF("focus_offset"));
	dofLineShader.setUniform("aperture", aperture);
	dofLineShader.setUniform("pointBrightness", pointBrightness);
	dofLineShader.setUniform("rgbBrightness", rgbBrightness);
	dofLineShader.setUniform("maxPointSize", panel.getValueF("maxPointSize"));
	
	
	float v[4];
	
	
	for(int i = 0; i < 144; i+= 2) {
		bool bInRun = false;
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			if (points[n].bVisibleThisFrame == true){
				if (bInRun == false){
					bInRun = true;	
					glBegin(GL_TRIANGLE_STRIP);
				}
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				int n0 = n;
				glNormal3f(1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
				glNormal3f(-1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
			
			
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
	}
	
	
	glTranslatef(-1500, 0, 0);
	
	for(int i = 0; i < 144; i+= 2) {
		bool bInRun = false;
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			if (points[n].bVisibleThisFrame == true){
				if (bInRun == false){
					bInRun = true;	
					glBegin(GL_TRIANGLE_STRIP);
				}
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				int n0 = n;
				glNormal3f(1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
				glNormal3f(-1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
				
				
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
	}
	
	
	glTranslatef(3000, 0, 0);
	
	for(int i = 0; i < 144; i+= 2) {
		bool bInRun = false;
		for(int j = 0; j < 176; j++) {
			int n = (j  + i * 176);
			if (points[n].bVisibleThisFrame == true){
				if (bInRun == false){
					bInRun = true;	
					glBegin(GL_TRIANGLE_STRIP);
				}
				//ofSetColor(points[n].color.x * 255.0, points[n].color.y * 255.0, points[n].color.z * 255.0, 200.0);
				int n0 = n;
				glNormal3f(1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
				glNormal3f(-1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
				
				
				
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
	}
	
		
	
	
	//
//	int step = 1; //(int)ofMap(sin(ofGetElapsedTimef()*20), -1,1, 1,8);
//	for(int i = 0; i < 144; i+= step) {
//		bool bInRun = false;
//		
//		
//		ofxVec3f upNormal;
//		ofxVec3f sideNormal;
//		
//		
//		glBegin(GL_TRIANGLE_STRIP);
//		
//		
//		
//		//glBegin(GL_LINE_STRIP);
//		for(int j = 0; j < 176-1; j++) {
//			int n0 = (j  + i * 176);
//			int n1 = ((j+1)  + i * 176);
//			
//			
//			
//			
//			
//			
//			//for(int i = 0; i < n-1; i++) {
//				// make left/bottom vertex
//				
//			
//				ofxVec3f diff = points[n1].pos - points[n0].pos;
//				diff.normalize();
//				
//				//dofLineShader.setUniform("nextVertex", points[n1].pos.x, points[n1].pos.y, points[n1].pos.z);
//			
//			
//				//glVertexAttrib1f(side, -1);
//				//dofLineShader.setAttribute("nextVertex", 1.0f, 0.0f, 0.0f);
//			
//			v[0] = points[n0].color.x;
//			v[1] = points[n0].color.y;
//			v[2] = points[n0].color.z;
//			v[3] = 1.0;
//			
//			//ofSetColor(points[n].color.x*255.0,points[n].color.y*255.0,points[n].color.z*255.0, 160);
//			//glColor4fv(v);
//			
//			
//			
//
//			glNormal3f(1,0,0);
//			glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
//			glVertex3f(points[n0].pos.x, points[n0].pos.y, points[n0].pos.z);
//			
//			
//				//dofLineShader.setAttribute("sideAA", 1.0f);
//			glNormal3f(-1,0,0);
//			glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
//			//glColor4fv(v);
//				//glTexCoord3f(points[n1].pos.x, points[n1].pos.y, points[n1].pos.z);
//								//dofLineShader.setUniform("sideAA", 3.0f);
//				//make right/top vertex
//				//glVertexAttrib1f(side, +1);
//				//glVertexAttrib3f(nextVertex, points[i+1].x, points[i+1].y, points[i+1].z);
//				glVertex3f(points[n0].pos.x, points[n0].pos.y, points[n0].pos.z);
//			
//			//}
//			
//			
//			//
////			//  a
//////			b c d
//////			  e
////			
////			ofxVec3f ptsa[5];	
////			ofxVec3f ptsb[5];	
////			
////			ofxVec3f pta; pta.set(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, points[n0].pos.z - pointSet.midPointSmoothed.z);
////			ofxVec3f ptb; ptb.set(points[n1].pos.x - pointSet.midPointSmoothed.x, points[n1].pos.y - pointSet.midPointSmoothed.y, points[n1].pos.z - pointSet.midPointSmoothed.z);
////			
////			//printf("%f %f %f %f %f %f \n", pta.x, pta.x, pta.z, ptb.x, ptb.y, ptb.z);
////			
////			ofxVec3f diff = ptb-pta;
////			diff.normalize();
////			ofxVec3f up;
////			up.set(0,1,0);
////			ofxVec3f side;
////			side.set(0,0,1);
////			
////			
////			ofxVec3f oldUpNormal;
////			ofxVec3f oldSideNormal; 
////			if (j != 0){
////				oldUpNormal = upNormal;
////				oldSideNormal = sideNormal;
////			}
////			
////			ofxVec3f upNormal = diff.getCrossed(up);
////			ofxVec3f sideNormal = diff.getCrossed(side);
////			if (j == 0){
////				oldUpNormal = upNormal;
////				oldSideNormal = sideNormal;
////			}
////			
////			
////			ptsa[0] = pta + upNormal *0.0;
////			ptsa[1] = pta + sideNormal*0.0;
////			ptsa[2] = pta;
////			ptsa[3] = pta - sideNormal *0.0;
////			ptsa[4] = pta - upNormal *0.0;
////			
////			
////			ptsb[0] = ptb + upNormal *0.0;
////			ptsb[1] = ptb + sideNormal*0.0;
////			ptsb[2] = ptb;
////			ptsb[3] = ptb - sideNormal *0.0;
////			ptsb[4] = ptb - upNormal *0.0;
////			
////			//draw two quads.  use color to represent which direction to distort this point! 
////			
////			//
//////			glBegin(GL_LINES);
//////			glVertex3f(pta.x, pta.y, pta.z);
//////			glVertex3f(ptb.x, ptb.y, ptb.z);
//////			
//////			glEnd();
////			
////			glBegin(GL_QUADS);
////			
////			glNormal3f(oldSideNormal.x, oldSideNormal.y, oldSideNormal.z);
////			glVertex3f(ptsa[1].x, ptsa[1].y, ptsa[1].z);
////			
////			glNormal3f(-oldSideNormal.x, -oldSideNormal.y, -oldSideNormal.z);
////			glVertex3f(ptsa[3].x, ptsa[3].y, ptsa[3].z);
////			
////			glNormal3f(-sideNormal.x, -sideNormal.y, -sideNormal.z);
////			glVertex3f(ptsb[3].x, ptsb[3].y, ptsb[3].z);
////			
////			glNormal3f(sideNormal.x, sideNormal.y, sideNormal.z);
////			glVertex3f(ptsb[1].x, ptsb[1].y, ptsb[1].z);
////			
////			
////			glEnd();
//			
//			
//			//glBegin(GL_QUADS);
////			
////			glNormal3f(oldUpNormal.x, oldUpNormal.y, oldUpNormal.z);
////			glVertex3f(ptsa[0].x, ptsa[0].y, ptsa[0].z);
////			
////			glNormal3f(-oldUpNormal.x, -oldUpNormal.y, -oldUpNormal.z);
////			glVertex3f(ptsa[4].x, ptsa[4].y, ptsa[4].z);
////			
////			
////			glNormal3f(-upNormal.x, -upNormal.y, -upNormal.z);
////			glVertex3f(ptsb[4].x, ptsb[4].y, ptsb[4].z);
////			
////			glNormal3f(upNormal.x, upNormal.y, upNormal.z);
////			glVertex3f(ptsb[0].x, ptsb[0].y, ptsb[0].z);
////
////			glEnd();
////			
//			
//			
//			//
////			
////			//float pctz = ( points[n].pos.z - pointSet.bottom.z) / (pointSet.top.z - pointSet.bottom.z);
////			//pctz = powf(pctz, 3.0);
////			//int clr = ofMap(pctz, 0,1,50, 255);
////			
////			if (points[n].bVisibleThisFrame == true){
////				ofSetColor(200,200,200,100);
////				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y, points[n].pos.z - pointSet.midPointSmoothed.z);
////				
////			} else {
////				ofSetColor(200,200,200,100);
////				glVertex3f(points[n].pos.x - pointSet.midPointSmoothed.x, points[n].pos.y - pointSet.midPointSmoothed.y,  points[n].pos.z - pointSet.midPointSmoothed.z);
////				
////				
////			}
//			
//		}
//		//glEnd();	
//		glEnd();
//		
//	}
	
	dofLineShader.end();
	
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