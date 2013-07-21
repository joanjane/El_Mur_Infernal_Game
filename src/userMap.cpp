#include "userMap.h"

#define SKIPPING_PIXELS 4

void userMap::setUserMap(){
    for( int i=0; i<_rows*_cols; i++){
        int x = i * _widthCell;
        int y = i * _heightCell;
    }
	_kC.setup();
	if(getKinectControllerRef()->isKinectPlugged())	//if we have a kinect plugged in
		_simulationMode = false;
	else{
		_simulationMode = true;
		_simulatedImage = new ofImage();
		_simulatedImage->loadImage("data/img/main_logo.png");
	}
    return;
}

void userMap::updateUserMap(){
    for (int j=_rows-1; j >= 0; j--){//era rows
        for (int i=_cols-1; i >= 0; i--){
            //mirem si al interior de la cel·la esta el usuari
			_gMap.at((_rows*_cols-1)-(_cols * j + i)) = isCellFull(i,j);//era true
        }
    }
    return;
}

/**
* Busca moviment en la imatge dins de una casella del mapa
* Retorna true si hi ha moviment del jugador en la casella, false sinó
*
**/
int userMap::isCellFull(int xIndex, int yIndex){
    ofColor black(0, 0, 0); //color negre
	int count = 0;
	int scale = SKIPPING_PIXELS;
	for (int i=0; i<_widthCell; i=i+scale){
		for (int j=0; j<_heightCell; j=j+scale){
			if(_silueta->getColor(xIndex*_widthCell + i, yIndex*_heightCell + j) != black){ //busca en la casella un pixel no negre
				count++;
				if(count >= _widthCell/scale * _heightCell/scale * 0.5)
					return 1;                               //xIndex*width sera la cel·la que volem + i, sera el pixel de la cel·la
			}
		}
	}
    return 0;
}

void userMap::drawUserMap(){
    ofSetColor(ofColor::yellow);
    //setUserMap();
    //drawTxtCells();
}

string userMap::drawTxtCells(){

	//stringstream msg;
	msg = "";
    for (int j=0; j < _rows; j++){
        for (int i=0; i < _cols; i++){
            if(isCellFull(i,j)){
                //msg << " O ";
				msg += " 0 ";
            }
            else{
               // msg << " - ";
				msg += " - ";
            }
        }
        //msg << endl;
		msg += "\n";
    }
	return msg;
    //ofScale(1.2,1.2,1.2);
    //ofDrawBitmapString(msg.str(), 640, 0);
}

void userMap::update(){
	_kC.update();
	updateImage();
	updateUserMap();
}

/**
* Updates de frames of the cleaned image of user from kinect
*/
void userMap::updateImage(){
	if(!_simulationMode)
		_silueta = _kC.getMaskImage();
	else
		_silueta = _simulatedImage;
	_widthCell = _silueta->getWidth()/_cols;
	_heightCell = _silueta->getHeight()/_rows;
}

//return a ref!
GridMap& userMap::getUserMap(){
	return _gMap;
}

void userMap::keyPressed(int key){
	_kC.keyReleased(key);
}