#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	/*movie.setup("ircamImages/dance-a-chan.3dmd");
	movie.loadColorData("dvcamImages/dance-a-chan", "ad ");
	*/
	
	nMovies = 3;

	movies = new ofx3dMovie[nMovies];
	processors = new ofx3dMovieProcessor[nMovies];
	pointSets = new point3dSet[nMovies];
		
	pointSetSet.pointSets = new point3dSet[nMovies];	
	pointSetSet.nSets = nMovies;

	movies[0].setup("3dData/nocchi-gun.3dmd");
	movies[0].loadColorData("3dData/nocchi-gun", "ng ", false); // false = .tif, true = .png
	
	movies[1].setup("3dData/nocchi-gun.3dmd");
	movies[1].loadColorData("3dData/nocchi-gun", "ng ", false); // false = .tif, true = .png
	
	
	movies[2].setup("3dData/nocchi-gun.3dmd");
	movies[2].loadColorData("3dData/nocchi-gun", "ng ", false); // false = .tif, true = .png
	
	
	
	for (int i = 0; i < nMovies; i++){
		processors[i].setup();	
	}

	
	ofBackground(0,0,0);
	ofSetFrameRate(30);
	
	
	
	numScenes = 7;
	scenes = new baseScene*[numScenes];
	scenes[0] = new simpleScene();
	scenes[1] = new lineScene();
	scenes[2] = new movementScene();
	scenes[3] = new thickLineScene();
	scenes[4] = new paintScene();
	scenes[5] = new movementLineDOFScene();
	scenes[6] = new movementLineDOF2Scene();

	for (int i = 0; i < numScenes; i++){
		scenes[i]->setup();
		scenes[i]->pointSet = pointSets[0];	
	}
	
	currentScene = 0;
	
	
}





//--------------------------------------------------------------
void testApp::update(){
	
	
	// copy ALL processor settings over: 
	if (nMovies > 1){
		for (int i = 1; i < nMovies; i++){
			processors[i].panel.setValueF( "THRESHOLD", processors[0].panel.getValueF("THRESHOLD"));
			processors[i].panel.setValueF( "DEPTHSCALE", processors[0].panel.getValueF("DEPTHSCALE"));
			processors[i].panel.setValueF( "DEPTHSCALE_MIN", processors[0].panel.getValueF("DEPTHSCALE_MIN"));
			processors[i].panel.setValueF( "DEPTHSCALE_MAX", processors[0].panel.getValueF("DEPTHSCALE_MAX"));
			processors[i].panel.setValueF( "DEPTH_T_SMOOTHING", processors[0].panel.getValueF("DEPTH_T_SMOOTHING"));
			processors[i].panel.setValueI( "N_MEDIAN", processors[0].panel.getValueI("N_MEDIAN"));
			processors[i].panel.setValueB( "USE_COMPUTATED", processors[0].panel.getValueB("USE_COMPUTATED"));
			processors[i].panel.update();
		}
	}
	
	scenes[currentScene]->update();
	
	
	bool bDoStatistics = false;
	for (int i = 0; i < nMovies; i++){
		int frameNum = ofGetFrameNum();
		int totalFrameNum = movies[i].totalFrameNum;
		processors[i].update(&movies[i], frameNum % totalFrameNum);
		
		// now, move movies over! 
		
		if (i > 0){
			
			
			rotationMatrix.makeIdentityMatrix();
			translationMatrix.makeIdentityMatrix();
			rotationMatrix *= ofxMatrix4x4::newRotationMatrix(-i*0.1f, 0,1,0);
			translationMatrix *= ofxMatrix4x4::newTranslationMatrix(1000*i + processors[i].pointSet.midpt.x, processors[i].pointSet.midpt.y,processors[i].pointSet.midpt.z);
			
			
			for (int j = 0; j < 176*144; j++){
				
				ofxVec3f pos;
				pos.set(processors[i].pointSet.points[j].pos.x - processors[i].pointSet.midpt.x, processors[i].pointSet.points[j].pos.y - processors[i].pointSet.midpt.y, processors[i].pointSet.points[j].pos.z - processors[i].pointSet.midpt.z);
				
				pos = pos*rotationMatrix;
				pos = pos*translationMatrix;
				
				processors[i].pointSet.points[j].pos = pos;
				
			}
		}
		
		
		pointSets[i] = processors[i].pointSet;
		pointSetSet.pointSets[i] = pointSets[i];
		
		// calculate statistics on pointSetSet :)
		if (processors[i].bNewFrame == true){
			bDoStatistics = true;
		}
	}
	
	//----------------------------------------------------------------
	// SORRRRRRRRRRRRRRRRRRRRRY SORRY SORRY  :(
	
	if (bDoStatistics == true){
		
		pointSetSet.top.set(0,0,0);
		pointSetSet.bottom.set(0,0,0);
		bool bFirstPoint = true;
		
		for (int k = 0; k < nMovies; k++){
			for(int i = 0; i < 176; i++) {
				for(int j = 0; j < 144; j++) {
					int n = (i  + j * 176);
					if (pointSets[k].points[n].bVisibleThisFrame == true){
						if (bFirstPoint == true){
							bFirstPoint = false;
							pointSetSet.top = pointSets[k].points[n].pos;
							pointSetSet.bottom = pointSets[k].points[n].pos;
						} else {
							pointSetSet.top.x = MAX(pointSetSet.top.x, pointSets[k].points[n].pos.x);
							pointSetSet.top.y = MAX(pointSetSet.top.y, pointSets[k].points[n].pos.y);
							pointSetSet.top.z = MAX(pointSetSet.top.z, pointSets[k].points[n].pos.z);
							pointSetSet.bottom.x = MIN(pointSetSet.bottom.x, pointSets[k].points[n].pos.x);
							pointSetSet.bottom.y = MIN(pointSetSet.bottom.y, pointSets[k].points[n].pos.y);
							pointSetSet.bottom.z = MIN(pointSetSet.bottom.z, pointSets[k].points[n].pos.z);
						}
					}
				}
			}
		}
		pointSetSet.midpt.set(0,0,0);
		pointSetSet.midpt.x = (pointSetSet.top.x+pointSetSet.bottom.x)/2.0;
		pointSetSet.midpt.y = (pointSetSet.top.y+pointSetSet.bottom.y)/2.0;
		pointSetSet.midpt.z = (pointSetSet.top.z+pointSetSet.bottom.z)/2.0;
		pointSetSet.midPointSmoothed = 0.97f * pointSetSet.midPointSmoothed + 0.03f * pointSetSet.midpt;
	}
	//----------------------------------------------------------------
	
		
	for (int i = 0; i < numScenes; i++){
		scenes[i]->pointSet = pointSets[0];
		scenes[i]->pointSetSet = pointSetSet;
	}

	for (int i = 0; i < numScenes; i++){
		scenes[i]->control.bDrawBox = false;
	}
	
}



//--------------------------------------------------------------
void testApp::draw(){
	
	processors[0].draw();
	
	scenes[currentScene]->draw();
	
	//ofSetColor(255,255,255);
	//movie.colorDataTexture.draw(mouseX, mouseY);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if (key == OF_KEY_LEFT){
		currentScene--;
		if (currentScene < 0){
			currentScene += numScenes;	
		}
	} else if (key == OF_KEY_RIGHT){
		currentScene ++;
		currentScene %= numScenes;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	processors[0].mouseMoved(x,y);
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	processors[0].mouseDragged(x,y,button);
	
	scenes[currentScene]->mouseDragged(x,y,button);
	
	if (x > 300 && x < 700)
	for (int i = 0; i < numScenes; i++){
		scenes[i]->control.rotateX = mouseX;
		scenes[i]->control.rotateY = mouseY;
		
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	processors[0].mousePressed(x,y,button);
	scenes[currentScene]->mousePressed(x,y,button);



}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	processors[0].mouseReleased(x,y,button);
	scenes[currentScene]->mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

