#ifndef _OFX_3D_PROCESSOR
#define _OFX_3D_PROCESSOR

#include "ofMain.h"
#include "ofx3dMovie.h"
#include "ofxCvGrayscaleImage.h"
#include "ofx3DGraphics.h"
#include "ofxControlPanel.h"
#include "point3d.h"



class ofx3dMovieProcessor {
public:
	
	
	float globalMinDepth;
	float globalMaxDepth;
	unsigned char * depthAsImage;
	unsigned char * depthPixels;
	unsigned char * temporalBlur;
	ofTexture		depthTexture;
	ofxCvGrayscaleImage depthCv;
	ofxCvGrayscaleImage depthCvSmoothed;
	
	
	point3dSet			pointSet;
	point3d	*			points;
	ofxControlPanel		panel;
	float				threshold;
	float				depthScale;

	
	int					lastFrame;
	
	void update(ofx3dMovie * movie, int frame);
	void setup();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};

#endif







