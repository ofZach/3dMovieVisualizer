#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	/*movie.setup("ircamImages/dance-a-chan.3dmd");
	movie.loadColorData("dvcamImages/dance-a-chan", "ad ");
	*/
	
	
	
	movie.setup("3dData/nocchi-gun.3dmd");
	movie.loadColorData("3dData/nocchi-gun", "ng ", false); // false = .tif, true = .png
	


	
	ofBackground(0,0,0);
	ofSetFrameRate(30);
	
	processor.setup();
	
	numScenes = 4;
	scenes = new baseScene*[numScenes];
	scenes[0] = new simpleScene();
	scenes[1] = new lineScene();
	scenes[2] = new movementScene();
	scenes[3] = new thickLineScene();


	for (int i = 0; i < numScenes; i++){
		scenes[i]->setup();
		scenes[i]->pointSet = pointSet;	
	}
	
	currentScene = 0;
	
}





//--------------------------------------------------------------
void testApp::update(){
	
	
	
	scenes[currentScene]->update();
	
	
	
	int frameNum = ofGetFrameNum();
	int totalFrameNum = movie.totalFrameNum;
	processor.update(&movie, frameNum % totalFrameNum);
	pointSet = processor.pointSet;
	
	for (int i = 0; i < numScenes; i++){
		scenes[i]->pointSet = processor.pointSet;
		
		
	}
	
	
	
	for (int i = 0; i < numScenes; i++){
		scenes[i]->control.bDrawBox = false;
	}
	
}



//--------------------------------------------------------------
void testApp::draw(){
	
	processor.draw();
	
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
	processor.mouseMoved(x,y);
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	processor.mouseDragged(x,y,button);
	
	scenes[currentScene]->mouseDragged(x,y,button);
	
	if (x > 300 && x < 700)
	for (int i = 0; i < numScenes; i++){
		scenes[i]->control.rotateX = mouseX;
		scenes[i]->control.rotateY = mouseY;
		
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	processor.mousePressed(x,y,button);
	scenes[currentScene]->mousePressed(x,y,button);



}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	processor.mouseReleased(x,y,button);
	scenes[currentScene]->mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

