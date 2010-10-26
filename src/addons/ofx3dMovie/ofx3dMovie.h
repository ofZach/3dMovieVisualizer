#pragma once

#include "ofMain.h"
#include "dataContainer.h"
#include "ofxDirList.h"

class ofx3dMovie {
	
	public :
	
	dataContainer		container;
	int					totalFrameNum;
	string				fileName;
	bool				debug_mode;
	FILE				* datas;
	
	vector <ofImage>	colorData;
	bool				bHaveColorData;
	ofTexture			colorDataTexture;
	unsigned char *		colorDataEmpty;
	
	
	
	void loadColorData(string _folderName, string fileNameSequence, bool bPng = true){
		
		
		ofxDirList colorDataList;
		int numImage = colorDataList.listDir(_folderName);
		
		if (numImage > 0){
			bHaveColorData = true;	
		}
		
		for (int i = 0; i < numImage; i++){
			string fileNameNumber = ofToString(i);
			if (fileNameNumber.length() < 3){
				int diff = 3 - fileNameNumber.length();
				for (int j = 0; j < diff; j++){
					fileNameNumber = "0" + fileNameNumber;
				}
			}
			
			ofImage temp;
			temp.setUseTexture(false);	
			string extension = bPng == true ? ".png" : ".tif";
			temp.loadImage(_folderName + "/" + fileNameSequence + fileNameNumber + extension);
			string tempStr = "dvcamImages/dance-a-chan/" + fileNameSequence + fileNameNumber + extension;
			temp.setImageType(OF_IMAGE_COLOR);
			colorData.push_back(temp);
		}
		
		if (colorData.size() > 0){
			colorDataTexture.allocate(352, 288, GL_RGB);
		}
	}
	
	
	
	void setup(string _fileName, bool _debug_mode = true) {
		
		colorDataEmpty  = new unsigned char[POINTS_LENGTH * 3];
		bHaveColorData	= false;
		fileName		= _fileName;
		debug_mode		= _debug_mode;
		totalFrameNum = 0;
		char buf[256];
		
		sprintf(buf, "%s%s", ofToDataPath("./").c_str(), _fileName.c_str());
		datas = fopen(buf, "rb");
		if(datas != NULL) {
			if(debug_mode) cout << "ofx3dMovieLoader::setup() | file open success. : " << buf << endl;
			fseek(datas, 0L, SEEK_END);
			totalFrameNum = ftell(datas) / containerSize;
			if(debug_mode) cout << "#" << " frame count is " << totalFrameNum << endl;
			fseek(datas, 0L, SEEK_SET);
			
			fread(&(container), containerSize, 1, datas);
		} else {
			if(debug_mode) cout << "ofx3dMovieLoader::setup() | file open error. : can't open " << buf << endl;
			memset(&(container), 0, containerSize * POINTS_LENGTH);
		}
		
	}
	
	inline dataContainer *getContainerAtIndexOfFrame(int _frameNum) {
		
		
		if(_frameNum < 0) {
			if(debug_mode) cout << "ofx3dMovieLoader::get*AtIndexOfFrame() | out of range : '_frameNum' = " << _frameNum << endl;
			_frameNum = 0;
		} else if(totalFrameNum <= _frameNum) {
			if(debug_mode) cout << "ofx3dMovieLoader::get*AtIndexOfFrame() | out of range : '_frameNum' = " << _frameNum
				<< ". number of frame at "  << " is " << totalFrameNum << "." << endl;
			_frameNum = totalFrameNum - 1;
		}
		
		if(fseek(datas, _frameNum * containerSize, SEEK_SET) == 0) {
			fread(&(container), containerSize, 1, datas);
		}
		
		return &(container);
	}
	
	inline float *getDepthAtIndexOfFrame(int _totalFrameNum) {
		return getContainerAtIndexOfFrame(_totalFrameNum)->depth;
	}
	
	inline unsigned char *getBrightnessAtIndexOfFrame(int _totalFrameNum) {
		return getContainerAtIndexOfFrame( _totalFrameNum)->brightness;
	}
	
	inline unsigned char *getColorAtIndexOfFrame(int _totalFrameNum) {
		
		if (_totalFrameNum >= 0 && _totalFrameNum < colorData.size()){
			return colorData[_totalFrameNum].getPixels();
		} else {
			return colorDataEmpty;
		}
	}
	
	
	
};