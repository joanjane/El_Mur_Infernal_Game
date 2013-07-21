#ifndef _PLAYER
#define _PLAYER

#pragma once

#include "Grid.h"
#include "userMap.h"
#include "ofxXmlSettings.h"

class Player: public Grid
{
	public:
		Player(int rows, int cols, Model* m, ofVec3f p, string color); 
		void update();
		void draw();
		void keyPressed(int key);
		userMap* getUserMapRef(){return _uM;}
		void savePosture(string filename="postures.xml");
		bool isReady(){return _isReady;}
		void isReady(bool b){_isReady = b;}
		int capturesDone(){return _capturesDone;}

	private:
		userMap* _uM;	//delete!
		ofxXmlSettings XML;
		bool _isReady;
		int _capturesDone;
};
#endif
