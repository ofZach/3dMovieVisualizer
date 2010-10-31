#pragma once

#include "ofMain.h"
#include "point3d.h"


class sceneControl {
	
	public: 
	
	float rotateX;
	float rotateY;
	bool bDrawBox;
	
	
};


class baseScene {

public:

	baseScene() { }

	virtual void setup(){}
	virtual void update(){}
	virtual void draw(){}
	
	virtual void mousePressed(int x, int y, int button){}
	virtual void mouseDragged(int x, int y, int button){}
	virtual void mouseReleased(){}
	

	// key board & mouse
	virtual void keyPressed(int key) {}
	virtual void keyReleased(int key){}
	virtual void mouseMoved(int wx, int wy, int x, int y ){}
	virtual void mouseDragged(int wx, int wy, int x, int y, int button){}
	virtual void mousePressed(int wx, int wy, int x, int y, int button){}
	virtual void mouseReleased(int wx, int wy, int x, int y, int button){}


	point3dSet					pointSet;
	point3dSetSet				pointSetSet;
	sceneControl				control;
	
	
};
