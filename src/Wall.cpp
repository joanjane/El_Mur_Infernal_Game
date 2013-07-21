#include "Wall.h"

Wall::Wall(int rows, int cols, Model* m, ofVec3f p, ofVec3f playerPos, string color): Grid(rows, cols, m, p, color)
{
	_isWallSet = false;
	setPlayerPosition(playerPos);
	_disableWall = false;
	velocityFactor = 5;
	_phase = -1;
	_isAWall = true;

#ifdef timeOF
	_startTime = ofGetElapsedTimef(); //time with OF. SUCKS
#else
	_startT0 = clock();
#endif
}

void Wall::update(){
	updateMovement();
}

void Wall::draw(){
	if (!_disableWall){
		update();
		Grid::drawGrid();
	}
}

void Wall::updateMovement(){

	velocityFactor = 5;
	if(Grid::getPosition().y < 0){	//moviment desde abaix
		setTimeByPhase(0);
		ofVec3f p( _position.x, timeElapsed() * velocityFactor + _initialPos.y, _position.z);
		Grid::setPosition(p);
	} else if(Grid::getPosition().z > 15){	//moviment endavant
		setTimeByPhase(1);
		
		int distance = Grid::getPosition().z - _playerPosition.z + 3;
		if (distance > 0){
			ofVec3f p( Grid::getPosition().x, 0, -1 * timeElapsed() * velocityFactor + _initialPos.z);
			Grid::setPosition(p);
		} else{
			_disableWall = true;
		}
	} else if (Grid::getPosition().x > 0 || Grid::getPosition().x < 0){	//moviment desde esquerra
		setTimeByPhase(0);
		if(Grid::getPosition().x > 0){
			direction = -1;
			if(_isWallFromRight){
				ofVec3f p(0, 0, Grid::getPosition().z);
				Grid::setPosition(p);
			} else{
				float x = timeElapsed() * velocityFactor * direction;
				ofVec3f p(x + _initialPos.x, 0, _position.z);
				Grid::setPosition(p);
			}
		} else if(Grid::getPosition().x < 0){							//from right
			direction = 1;
			if(!_isWallFromRight){
				ofVec3f p(0, 0, Grid::getPosition().z);
				Grid::setPosition(p);
			} else{
				float x = timeElapsed() * velocityFactor * direction;
				ofVec3f p(x + _initialPos.x, 0, _position.z);
				Grid::setPosition(p);
			}
		}
		_initialPos.z = _position.z;
	} else{																//moviment endavant
		//CHECK ANY ERROR IN POSITION
		if (_initialPos.z > 15)
			_initialPos.z = 15;

		setTimeByPhase(1);
		int distance = Grid::getPosition().z - _playerPosition.z + 3;	//3 is the margin to make the wall disappear after crossing the player
		if (distance > 0){
			ofVec3f p( Grid::getPosition().x, Grid::getPosition().y, -1 * timeElapsed() * velocityFactor + _initialPos.z);
			Grid::setPosition(p);
		} else{
			_disableWall = true;
		}
	}
}

/**
* This method provides a way to update the map of the wall 
* with the collision of the player map. When a collision is
* detected, the wall map is updated with that info to display
* a different block colour.
*/
void Wall::updateCollisionData(GridMap& player){
	_gMap = _restore;
	GridMap c = Grid::checkCollisions(player);
	GridMap o = Grid::checkCorrectBlocks(player);
	for (int i=0; i < c.size(); i++){
		_gMap.at(c.at(i)) = 2;
	}
	for (int i=0; i < o.size(); i++){
		_gMap.at(o.at(i)) = 3;
	}
}

void Wall::setWallForm(GridMap& g){
	_gMap = g;
	_restore = g;
	_isWallSet = true;
}

void Wall::generateFullWall(){
	if(!_isWallSet){
		_isWallSet = true;
		for (int i = 0; i < _cols; i++){
			for (int j = 0; j < _rows; j++){
				_gMap.at(_cols * j + i) = 1;
			}
		}
	}
}


/** Used to get the time every start of a new phase movement.
*	Need to be called swapping a number between phases, like
*	first call with 0 and on the next phase, call with 1, then 0, 1...
*/
void Wall::setTimeByPhase(int p){
	if (_phase != p){
		if (p == 0){
#ifdef timeOF
			_startTime = ofGetElapsedTimef();
#else
			//time(&_startT);
			_startT0 = clock();
#endif
		} else if (p == 1){ //incrementar o decrementar la x
#ifdef timeOF
			_startTime = ofGetElapsedTimef();
#else
			//time(&_startT);
			_startT0 = clock();
#endif
		} else{
#ifdef timeOF
			_startTime = ofGetElapsedTimef();
#else
			//time(&_startT);
			_startT0 = clock();
#endif
		}
	}
	_phase = p;
}

float Wall::getDistanceFromPlayerPercentage(){
	float currentDistance = Grid::getPosition().z;// - _playerPosition.z;
	float totalDistance = _initialPosition - _playerPosition.z;
	if (currentDistance < 0){
		currentDistance = 0;
	}
	float d =  1 - (currentDistance / totalDistance);
	return d;
}

double Wall::timeElapsed(){
#ifdef timeOF
		return (double)(ofGetElapsedTimef() - _startTime);
#else
		double t = (double)((double)clock() - (double)_startT0)/CLOCKS_PER_SEC;
		t = t - _timePaused;	//if has been paused
		return t;

#endif
}