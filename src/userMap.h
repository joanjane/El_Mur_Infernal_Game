#ifndef USERMAP_H
#define USERMAP_H

#include <vector>
#include <iostream>
#include "ofMain.h"
#include "kinectController.h"

typedef std::vector<int> GridMap;
using namespace std;

class userMap
{
    public:
        userMap(int x, int y){
            _rows = x;
            _cols = y;
			for (int i = 0; i < _cols; i++){
				for (int j = 0; j < _rows; j++){
					_gMap.push_back(0);//era false
				}
			}
        }
        /*
        ~userMap(){
            for (i=0; i<_rows*_cols; i++){
                delete cellMap.at(i);
            }
            delete *silueta;
        }
        */
		void updateImage();
        int rows(){return _rows;}
        int cols(){return _cols;}
        void dimmension(int x, int y){
            _rows = x;
            _cols = y;
        }
        void setUserMap();
		GridMap& getUserMap();
        void updateUserMap();
        void drawUserMap();             //dibuixa els quadrats ocupats
        void drawCellLines();           //dibuixa les linies de la cel·la
        string drawTxtCells();
        int isCellFull(int x, int y);  //mira si una cel·la en concret esta el usuari
		void update();
		void keyPressed(int key);
		string& getTxtCells(){return msg;}
		string msg;
		kinectController* getKinectControllerRef(){return &_kC;}
		ofImage* getSilueta()	{return _silueta;}

    protected:
        int _rows, _cols;
        int _widthCell, _heightCell;
        vector<ofRectangle*> cellMap;

        ofImage* _silueta;      //problema!!!

        int _xFilledCells[15];
        int _yFilledCells[10];

		kinectController _kC;
		GridMap _gMap;
		ofImage* _simulatedImage;
		bool _simulationMode;
    private:
};

#endif // USERMAP_H
