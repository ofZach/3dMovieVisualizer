#include "ofx3dMovieProcessor.h"

//--------------------------------------------------------------
void ofx3dMovieProcessor::setup(){
	

	globalMinDepth = 1000000;
	globalMaxDepth = -1000000;
	
	temporalBlur = new unsigned char[176*144];
	depthPixels = new unsigned char[176*144];
	depthTexture.allocate(176, 144, GL_LUMINANCE);
	
	depthCv.allocate(176, 144);
	depthCvSmoothed.allocate(176, 144);
	
	points = new point3d[176*144];
	pointSet.points = points;
	pointSet.nPoints = 176*144;
	
	panel.setup("control", 1200-300,150,300,700);
	panel.addPanel("3d control", 1, false);
	panel.setWhichPanel("3d control");
	panel.addSlider("threshold", "THRESHOLD", 0, 0, 255,  true);
	panel.addSlider("depthScale", "DEPTHSCALE", 1, 0.1, 40,  false);
	panel.addToggle("use computed min/max", "USE_COMPUTATED", false);
	panel.addSlider("minDepth", "DEPTHSCALE_MIN", 0, 0,8,  false);
	panel.addSlider("maxDepth", "DEPTHSCALE_MAX", 4, 0.1,10,  false);
	panel.addSlider("num median filterings", "N_MEDIAN", 1, 0,10,  true);
	panel.addSlider("temporal smoothing", "DEPTH_T_SMOOTHING", 0,0,1,  false);

	panel.loadSettings("3dProcessorSettings.xml");
	
	lastFrame = -1;
	
	bNewFrame = false;
}





//--------------------------------------------------------------
void ofx3dMovieProcessor::update(ofx3dMovie * movie, int frameNum){
	
	panel.update();
	
	threshold	= panel.getValueI("THRESHOLD");
	depthScale	= panel.getValueF("DEPTHSCALE");
	
	
	int totalFrameNum = movie->totalFrameNum;
	float *depth = movie->getDepthAtIndexOfFrame(frameNum % totalFrameNum);

	
	bool bUseComputed = 	panel.getValueB("USE_COMPUTATED");
	
	
	if (bUseComputed == true){
		for(int i = 0; i < 176; i++) {
			for(int j = 0; j < 144; j++) {
				float pz = depth[i + 176 * j];
				globalMinDepth = MIN(globalMinDepth, pz);
				globalMaxDepth = MAX(globalMaxDepth, pz);
				
			}
		}
		panel.setValueF("DEPTHSCALE_MIN", globalMinDepth);
		panel.setValueF("DEPTHSCALE_MAX", globalMaxDepth);

		
	} else {
		globalMinDepth = panel.getValueF("DEPTHSCALE_MIN");
		globalMaxDepth = panel.getValueF("DEPTHSCALE_MAX");
	}
	
	
	for(int i = 0; i < 176; i++) {
		for(int j = 0; j < 144; j++) {
			float pz = depth[i + 176 * j];
			depthPixels[i + 176 * j] = ofMap(pz, globalMinDepth, globalMaxDepth, 0, 255, true);
			if (depthPixels[i + 176 * j] < threshold) depthPixels[i + 176 * j] = 255;
		}
	}
	
	depthTexture.loadData(depthPixels, 176, 144, GL_LUMINANCE);	
	depthCv.setFromPixels(depthPixels, 176, 144);

	for (int i = 0; i < panel.getValueI("N_MEDIAN"); i++){
		
		// TODO make an inplace media filtering in ofxCvGray image.
		
		cvSmooth(depthCv.getCvImage(),depthCvSmoothed.getCvImage(),CV_MEDIAN,3);
		depthCvSmoothed.flagImageChanged();
		depthCv = depthCvSmoothed;
	}
	
	
	//depthCv.blur(5);
	
	
	float amount = panel.getValueF("DEPTH_T_SMOOTHING");
	
	
	int prevFrame = lastFrame;
	lastFrame = frameNum;
	
	if (prevFrame != frameNum){
		bNewFrame = true;
		unsigned char * pix = depthCv.getPixels();
		for (int i = 0; i < 176*144; i++){
			temporalBlur[i] = amount * temporalBlur[i] + (1-amount) * pix[i];
		}
	} else {
		bNewFrame = false;	
	}
	unsigned char *brightness = movie->getBrightnessAtIndexOfFrame(frameNum % totalFrameNum);
	unsigned char *color = movie->getColorAtIndexOfFrame(frameNum % totalFrameNum);
	
	
	movie->colorDataTexture.loadData(movie->getColorAtIndexOfFrame(frameNum % totalFrameNum), 176*2, 144*2, GL_RGB);
	
	
	for(int i = 0; i < 176; i++) {
		for(int j = 0; j < 144; j++) {
			int cn = (i*2 + j*2 * 176*2);
			int dn = (i  + j * 176);
			float bri = color[cn*3] + color[cn*3+1] + color[cn*3+2];
			
			float pz =temporalBlur[i + 176 * j]; // * calibDepth;
			
			
			if ((bri / 3.0) > 5){
				
				points[dn].color.set(color[cn*3] / 255.0 + 0.3, color[cn*3+1] / 255.0 + 0.3, color[cn*3+2] / 255.0 + 0.3);
				points[dn].pos.set((i / 176.0 * ofGetWidth() - ofGetWidth()/2.0)*3, (j / 144.0 * ofGetHeight() - ofGetHeight()/2.0)*3, -depthScale * pz);
				points[dn].bVisibleThisFrame = true;
				
				//glColor3f(color[cn*3] / 255.0 + 0.3, color[cn*3+1] / 255.0 + 0.3, color[cn*3+2] / 255.0 + 0.3);
				//glVertex3f((i / 176.0 * ofGetWidth() - ofGetWidth()/2.0)*3, (j / 144.0 * ofGetHeight() - ofGetHeight()/2.0)*3, depthScale * pz);
			} else {
				
				points[dn].color.set(color[cn*3] / 255.0 + 0.3, color[cn*3+1] / 255.0 + 0.3, color[cn*3+2] / 255.0 + 0.3);
				points[dn].pos.set((i / 176.0 * ofGetWidth() - ofGetWidth()/2.0)*3, (j / 144.0 * ofGetHeight() - ofGetHeight()/2.0)*3, -depthScale * pz);
				points[dn].bVisibleThisFrame = false;
				
			}
			
			
		}
	}
	
	
	
	pointSet.top.set(0,0,0);
	pointSet.bottom.set(0,0,0);
	bool bFirstPoint = true;
	for(int i = 0; i < 176; i++) {
		for(int j = 0; j < 144; j++) {
			
			
			int n = (i  + j * 176);
			
			if (points[n].bVisibleThisFrame == true){
				if (bFirstPoint == true){
					bFirstPoint = false;
					pointSet.top = points[n].pos;
					pointSet.bottom = points[n].pos;
				} else {
					pointSet.top.x = MAX(pointSet.top.x, points[n].pos.x);
					pointSet.top.y = MAX(pointSet.top.y, points[n].pos.y);
					pointSet.top.z = MAX(pointSet.top.z, points[n].pos.z);
					pointSet.bottom.x = MIN(pointSet.bottom.x, points[n].pos.x);
					pointSet.bottom.y = MIN(pointSet.bottom.y, points[n].pos.y);
					pointSet.bottom.z = MIN(pointSet.bottom.z, points[n].pos.z);
				}
			}
		}
	}
	pointSet.midpt.set(0,0,0);
	pointSet.midpt.x = (pointSet.top.x+pointSet.bottom.x)/2.0;
	pointSet.midpt.y = (pointSet.top.y+pointSet.bottom.y)/2.0;
	pointSet.midpt.z = (pointSet.top.z+pointSet.bottom.z)/2.0;
	pointSet.midPointSmoothed = 0.97f * pointSet.midPointSmoothed + 0.03f * pointSet.midpt;

	
}



//--------------------------------------------------------------
void ofx3dMovieProcessor::draw(){
	
	ofSetColor(255,255,255);
	depthCv.draw(1200-300,0);
	panel.draw();

}

//--------------------------------------------------------------
void ofx3dMovieProcessor::keyPressed(int key){

}

//--------------------------------------------------------------
void ofx3dMovieProcessor::keyReleased(int key){

}

//--------------------------------------------------------------
void ofx3dMovieProcessor::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofx3dMovieProcessor::mouseDragged(int x, int y, int button){	
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ofx3dMovieProcessor::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);

}

//--------------------------------------------------------------
void ofx3dMovieProcessor::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void ofx3dMovieProcessor::windowResized(int w, int h){

}