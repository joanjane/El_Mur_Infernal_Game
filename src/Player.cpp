#include "Player.h"

Player::Player(int rows, int cols, Model* m, ofVec3f p, string color): Grid(rows, cols, m, p, color)
{
	_uM = new userMap(rows,cols);
	_uM->setUserMap();
	_isReady = false;
	_isAWall = false;
	_capturesDone = 0;
}

void Player::update(){
	_uM->update();
	_gMap = _uM->getUserMap();
	if(!_isReady){
		for( int i=0; i<_gMap.size(); i++){
			if(_gMap.at(i) == 1){
				_isReady = true;
				break;
			}
		}
	}
}

void Player::draw(){
	Grid::drawGrid();
}

void Player::keyPressed(int key){
	//_uM->keyPressed(key);
	
	if(key == ' '){
		savePosture();
	}
}

void Player::savePosture(string filename){
	int lastTagNumber = XML.addTag("POSTURE");
	if(XML.pushTag("POSTURE", lastTagNumber)){
	
		int tagNum = XML.addTag("DIMENSIONS");
		XML.pushTag("DIMENSIONS");

		XML.addValue("ROWS", _rows);
		XML.addValue("COLS", _cols);
		XML.popTag();

		int tagNum2 = XML.addTag("VALUES");
		XML.pushTag("VALUES");
		for (int i=0; i < _gMap.size(); i++){		
			tagNum2=XML.addValue("PT", _gMap.at(i));	
		}
		XML.popTag();
	}
	XML.popTag();

	XML.saveFile(filename);
	_capturesDone++;
}