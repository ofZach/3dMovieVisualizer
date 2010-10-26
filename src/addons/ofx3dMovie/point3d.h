/*
 *  dataContainer.h
 *  mesa_editor
 *
 *  Created by 2bit on 10/04/04.
 *  Copyright 2010 4nchor5 la6. All rights reserved.
 *
 */

#pragma once
#include "ofxVectorMath.h"

class point3d {
public: 
	ofPoint pos;
	ofPoint color;
	bool bVisibleThisFrame;
};

class point3dSet {

	public: 
	
		point3d * points;
		int nPoints;
		ofPoint bottom;
		ofPoint top;
		ofxVec3f	midpt;
		ofxVec3f	midPointSmoothed;
	
};