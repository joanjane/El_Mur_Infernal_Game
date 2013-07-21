#ifndef _GRID
#define _GRID

#pragma once

#include "ofxAssimpModelLoader.h"
#include "Model.h"
#include <vector>
#include <time.h>

typedef std::vector<int> GridMap;

#define BLOCK_SEPARATION 500;

class Grid
{
	public:
		Grid(int rows, int cols, Model* m, ofVec3f p, string color); 
		~Grid(){
			delete _model;
		} 
		virtual void draw() =0;
		virtual void update() =0;
		void setPosition(ofVec3f p);	//sets the position of the hole body
		ofVec3f getPosition();
		void drawGrid();
		void resetGridMap();
		GridMap checkCollisions(GridMap& gM);
		GridMap checkCorrectBlocks(GridMap& gM);
		void setStartPoint(ofVec3f p);
		GridMap getMap()	{ return _gMap;}
		void loadMap(GridMap& g);
		int notUsedBlocks(){return _notUsedBlocks;}
		int collisionNumber(){return _collisionNumber;}
		int correctBlocks(){return _correctBlocks;}
		void generateRandomGrid();
		float amountOfMovement(){return _amountOfMovement;}
		void restoreGrid();
		void enableRainbowMode(){_rainbowMode=true;}
		void disableRainbowMode(){_rainbowMode=false;}
		bool rainbowMode(){return _rainbowMode;}
		void generateRandomColor();
		void pauseResume(){
			_isPaused=!_isPaused;
			if(_isPaused){
				_pausedT = clock();
			} else{
				if(_pausedT != 0)	//if has been paused at least one time
					_timePaused += ((double)clock() - _pausedT)/CLOCKS_PER_SEC;
			}
		}
		bool isPaused(){return _isPaused;}
		
	protected:
		int _rows, _cols;//mida parets
		GridMap _gMap;
		GridMap _resetMap;
		GridMap _restore;
		GridMap _previousMap;
		Model* _model;
		ofVec3f _position;			//posicio absoluta del punt del primer block
		ofVec3f _startPoint;
		ofColor _cubeColor;
		int _notUsedBlocks;
		int _collisionNumber;
		int _correctBlocks;
		bool _isGridSet;
		float _amountOfMovement;
		bool _isAWall;
		bool _rainbowMode;
		bool _isPaused;
		double _timePaused;
		clock_t _pausedT;
};
#endif
