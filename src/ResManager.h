#ifndef _MY_RESOURCE_MANAGER
#define _MY_RESOURCE_MANAGER

#pragma once

#include "ofxAssimpModelLoader.h"
#include <map>
#include <vector>
#include "Model.h"
#include "ofxXmlSettings.h"
#include "Wall.h"
//#include "ScoreController.h"

typedef std::list<ofVec3f *> WallsPositions;	//punt del primer block de cada wall
typedef std::vector<int> GridMap;
typedef std::vector<Wall *> Walls;

class MyResourceManager
{
	public:
		MyResourceManager(void);
		~MyResourceManager(void);
		Model& getModelByName(std::string name);
		void setAModel(string fileName, string mapName);
		void addAWall(ofVec3f* p);
		ofVec3f& getWallPositionByIndex(int index);
		void updateWallsPositions(int index, ofVec3f* p);
		void deleteFirstWall();
		int numberOfWalls();
		void loadMapsFromXML(string filePath="postures.xml");
		void createWallsFromMaps(ofVec3f playerPos);
		Walls& getWallsRef()	{return _walls;}

	private:
		map<string, Model*> _modelsMap;
		WallsPositions _wallsPos;
		vector<GridMap> _maps;
		Walls _walls;
		int _cols, _rows;
};
#endif
