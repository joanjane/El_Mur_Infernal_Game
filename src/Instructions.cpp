#include "Instructions.h"

/**
* This class loads images named from 1 to N
* and make a slideshow drawing of each one
*/
Instructions::Instructions(void)
{
	_hideSlides = false;
	_currentNumberOfSlide = 0;
	int totalSlidesNumber = 4;
	string path = "instructions/";
	string imageExtension = ".PNG";
	for (int j=0; j < totalSlidesNumber; j++){
		ofImage* im= new ofImage();
		if(im->loadImage(path + ofToString(j+1)+imageExtension)){
			_slides.push_back(im);
			std::cout << "Loaded image file " << path << j+1 << imageExtension << endl;
		}
	}	
}

void Instructions::draw(){
	if(!_hideSlides)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
			_slides.at(_currentNumberOfSlide)->draw(0,0,0,ofGetWidth(), ofGetHeight());
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
}

