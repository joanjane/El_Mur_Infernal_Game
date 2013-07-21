#include "Grid.h"
#include "ofMain.h"

#define CUBE_DISTANCE 0.4
Grid::Grid(int rows, int cols, Model* m, ofVec3f p, string color){
	_rows = rows;
	_cols = cols;
	_model = m;
	_startPoint = p;
	setPosition(_startPoint);
	for (int i = 0; i < _cols; i++){
		for (int j = 0; j < _rows; j++){
			_resetMap.push_back(0);
		}
	}
	_gMap = _resetMap;
	_restore = _gMap;
	_previousMap = _gMap;

	if (color == "blue"){
		_cubeColor = ofColor(36, 114, 213);
	}else if(color == "yellow"){
		_cubeColor = ofColor(217,213,36);
	} else if(color == "grey"){
		_cubeColor = ofColor(100,100,100);
	}

	_isGridSet = false;

	_notUsedBlocks = 0;
	_collisionNumber = 0;
	_correctBlocks = 0;
	disableRainbowMode();
	_isPaused = false;
	_timePaused = 0;
}

void Grid::drawGrid(){
	_notUsedBlocks = 0;
	_collisionNumber = 0;
	_correctBlocks = 0;
	_amountOfMovement = 0;
	for (int i = 0; i < _cols; i++){
		for (int j = 0; j < _rows; j++){
				ofPushMatrix();
					ofTranslate(_position.x + i * CUBE_DISTANCE, _position.y + j * CUBE_DISTANCE, _position.z + -1 );
					int currentBlock = _gMap.at(_cols * j + i);
					ofPushStyle();
					if (currentBlock == 1){
						if(!rainbowMode())
							ofSetColor(_cubeColor);	//color normal
						else
							generateRandomColor();
					} else if(currentBlock == 2){
						_collisionNumber++;
						ofSetColor(255,0,0);	//color colisio
					} else if(currentBlock == 3){
						_correctBlocks++;
						ofSetColor(20,160,20);	//color oportunitat de guanyar mes punts
					} else if(currentBlock == 0){
						 _notUsedBlocks++;
						 ofSetColor(5,5,5);		//no filled block (of the hole) color
					}
					if(!_isAWall && currentBlock != 0 || _isAWall){
						ofScale(0.11,0.11,0.11);
						_model->drawModel();
					}
					////////////////AMOUNT OF MOVEMENT CALCULATION//////////////
					if(_previousMap.at(_cols * j + i) != currentBlock){
						_amountOfMovement++;
					}
					////////////////AMOUNT OF MOVEMENT CALCULATION//////////////
					ofPopStyle();
				ofPopMatrix();
		}
	}
	_previousMap = _gMap;
	//_amountOfMovement = _amountOfMovement / (_rows*_cols);
}

void Grid::resetGridMap(){
	_gMap = _resetMap;
}

GridMap Grid::checkCollisions(GridMap& gM){
	//int numberCollisions = 0;
	vector<int> collisionBlocksIndex;
	for (int i = 0; i < _cols; i++){
		for (int j = 0; j < _rows; j++){
			//int currentIndex = _cols * j + i;
			int currentIndex = _cols * j + i;
			if (_gMap.at(currentIndex) != 0 && gM.at(currentIndex) != 0){
				//numberCollisions++;
				collisionBlocksIndex.push_back(currentIndex);
			}
		}
	}
	//return numberCollisions;
	return collisionBlocksIndex;
}

GridMap Grid::checkCorrectBlocks(GridMap& gM){
	//int numberCollisions = 0;
	vector<int> collisionBlocksIndex;
	for (int i = 0; i < _cols; i++){
		for (int j = 0; j < _rows; j++){
			//int currentIndex = _cols * j + i;
			int currentIndex = _cols * j + i;
			if (_gMap.at(currentIndex) == 0 && gM.at(currentIndex) == 1){
				//numberCollisions++;
				collisionBlocksIndex.push_back(currentIndex);
			}
		}
	}
	//return numberCollisions;
	return collisionBlocksIndex;
}

void Grid::setPosition(ofVec3f p){
	if (p.x < 5 && p.x > -5)
		_position = p;
	_position = p;
}

ofVec3f Grid::getPosition(){
	return _position;
}

void Grid::loadMap(GridMap& g){
	_restore = g;
	_gMap = g;
}

void Grid::generateRandomGrid(){
	if(!_isGridSet){
		_isGridSet = true;
		for (int i = 0; i < _cols; i++){
			for (int j = 0; j < _rows; j++){
				if(rand() % 100 > 49){
					_gMap.at(_cols * j + i) = 1;
				} else{
					_gMap.at(_cols * j + i) = 0;
				}
			}
		}
		_restore = _gMap;
	}
}

void Grid::restoreGrid(){
	_gMap = _restore;
}

void Grid::generateRandomColor(){
	int r = rand() % 256;	//note: from 0 to 255
	int g = rand() % 256;
	int b = rand() % 256;
	ofSetColor(r,g,b);
}