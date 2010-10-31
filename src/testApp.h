#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofx3dMovie.h"
#include "ofxCvGrayscaleImage.h"
#include "ofx3DGraphics.h"
#include "ofxControlPanel.h"
#include "point3d.h"
#include "ofx3dMovieProcessor.h"
#include "baseScene.h"
#include "simpleScene.h"
#include "lineScene.h"
#include "movementScene.h"
#include "thickLineScene.h"
#include "paintScene.h"
#include "movementLineDOFScene.h"
#include "movementLineDOF2Scene.h"

//#include <cairo/carioline.h>


class testApp : public ofBaseApp{
public:


	int							nMovies;
	point3dSetSet				pointSetSet;

	ofx3dMovie					* movies;
	ofx3dMovieProcessor			* processors;
	point3dSet					* pointSets;
 
	baseScene					** scenes;
	int							currentScene;
	int							numScenes;
	
	
	ofxMatrix4x4				rotationMatrix;
	ofxMatrix4x4				translationMatrix;
	
	
	void setup();
	void update();
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







