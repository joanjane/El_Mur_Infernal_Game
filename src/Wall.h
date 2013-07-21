#ifndef _WALL
#define _WALL

#pragma once

#include "Grid.h"
#include <time.h>
//#define timeOF

class Wall: public Grid
{
	public:
		Wall(int rows, int cols, Model* m, ofVec3f p, ofVec3f playerPos, string color); 
		void update();
		void draw();
		void generateRandomWall();
		void updateMovement();
		void setPlayerPosition(ofVec3f p){ _playerPosition = p;}
		void updateCollisionData(GridMap& player);
		void isWallSet(bool b){_isWallSet = b;}
		void setWallForm(GridMap& g);
		void setVelocityFactor(float v){ velocityFactor = v;}
		void generateFullWall();
		bool isWallDisabled() {return _disableWall;}
		void disableWall(){ _disableWall = true;}
		void enableWall(){
			_disableWall = false;
			//_startTime = ofGetElapsedTimef();
			_phase = -1;
			_initialPosition = _position.z;
			_initialPos = _position;
			if (_position.x < 0) _isWallFromRight = true;
			else				 _isWallFromRight = false;
		}
		void setTimeByPhase(int p);
		float getDistanceFromPlayerPercentage();
		double timeElapsed();

	private:
		bool _isWallSet;
		ofVec3f _playerPosition;
		bool _disableWall;
#ifdef timeOF
		float _startTime;
#else
		//time_t _startT;
		clock_t _startT0;
#endif
		float velocityFactor;
		float _initialPosition;
		ofVec3f _initialPos;
		bool _isWallFromRight;
		float debug;
		int _phase;
		int direction;
};
#endif