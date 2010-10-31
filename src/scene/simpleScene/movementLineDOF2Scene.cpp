/*
 *  movementLineDOF2Scene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "movementLineDOF2Scene.h"
#include "testApp.h"



void movementLineDOF2Scene::setup(){
		
	
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
	
	
	panel.loadSettings("movementLineDOF2SceneSettings.xml");
	
	
	depthPixels = new unsigned char [176 * 144];
	depthImage.setUseTexture(false);
	depthImage.allocate(176, 144);

/*	ofxCvGrayscaleImage depthImage;
	ofxCvContourFinder	contourFinder;
*/	
	numDivisions = 15;
	contourFinder = new ofxCvContourFinder[numDivisions];
	
	
}

void movementLineDOF2Scene::update(){
	
	panel.update();
	
	testApp * app = (testApp * ) ofGetAppPtr();
	
	int currLastFrame = app->processors[0].lastFrame;
	
	
	
	point3d * points = pointSet.points;
	
	
	if (currLastFrame != lastFrameNum){
		
		currFrame = app->processors[0].depthCv;
		
		
		
		
		for (int i = 0; i < numDivisions; i++){
			
			int divisionLine = ((float)(i+0) / (float)(numDivisions+1)) * 255;
			int divisionLine2 = ((float)(i+1) / (float)(numDivisions+1)) * 255;
			
			//cout << divisionLine << endl;
			depthImage = currFrame;
			depthImage.invert();
			
			
			unsigned char * pix = depthImage.getPixels();
			for (int j = 0; j < 144*176; j++){
				if (pix [j] >= divisionLine && pix [j] < divisionLine2){
					pix [j] = 255;
				} else {
					pix [j] = 0;	
				}
			}
			depthImage.setFromPixels(pix, 176, 144);
			
			//depthImage.blur(3);
			
			//depthImage.threshold(128);
			
			//depthImage.threshold(divisionLine);
			
			contourFinder[i].findContours(depthImage, 1, 176*144, 100, true, true);
			//cout << contourFinder[i].nBlobs << endl;
		}
				
		
		lastFrameNum = currLastFrame;
				
		
	}
	
	
		
		
	
}

void movementLineDOF2Scene::draw(){
	
	
	
	
	for (int i = 0; i < numDivisions; i++){
		contourFinder[i].draw(0,0);
	}
	
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
	
	
	for (int k = 0; k < numDivisions; k++){
		
		int nBlobs = contourFinder[k].nBlobs;
		
		
		for (int i = 0; i < nBlobs; i++){
			
			int nPts = contourFinder[k].blobs[i].nPts;
			
			glBegin(GL_TRIANGLE_STRIP);
			
			
			
			
			for (int j = 0; j < nPts; j++){
				
				
				int howRandom = ofMap(sin(ofGetElapsedTimef() * 5), -1,1, 0, nPts/2);
									  
									  
				int signme = ofRandom(0,1) > 0.5 ? -1 : 1;
				int who = j + (signme * ofRandom(0,howRandom));
				while (who < 0)  who += nPts;
				while (who > nPts)  who -= nPts;
				
				who %= nPts;
				if (who < 0) who = 0;
				
				float pct = (float)(k + 1) / (float)(numDivisions+3);
				ofPoint temp = contourFinder[k].blobs[i].pts[who];
				
				
				int x = (int)temp.x;
				int y = (int)temp.y;
				int n = (x  + y * 176);
				int n0 = n;
				
				
				glNormal3f(1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, pointSet.bottom.z + (pointSet.top.z - pointSet.bottom.z)*pct  - pointSet.midPointSmoothed.z  );
				
				glNormal3f(-1,0,0);
				glColor4f(points[n0].color.x,points[n0].color.y,points[n0].color.z,1);
				glVertex3f(points[n0].pos.x - pointSet.midPointSmoothed.x, points[n0].pos.y - pointSet.midPointSmoothed.y, pointSet.bottom.z + (pointSet.top.z - pointSet.bottom.z)*pct  - pointSet.midPointSmoothed.z  );
				
			}
			
			glEnd();
		}
	}
	
	/*
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
	*/
	
		
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