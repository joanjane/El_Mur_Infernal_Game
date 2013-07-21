#include "ResManager.h"

MyResourceManager::MyResourceManager(void)
{
	Model* m = new Model("models/roundedCube.obj");
	_modelsMap["cube"] = m;
	/*delete m;
	m = new Model("Rubik's Cube.3ds");
	_modelsMap["player"] = m;*/
	Model* m2 = new Model("models/plane.obj");
	_modelsMap["plane"] = m2;
}

MyResourceManager::~MyResourceManager(void)
{
	//deletes for map of models
	for( map<string, Model*>::iterator ii=_modelsMap.begin(); ii!=_modelsMap.end(); ++ii)
	{
	   delete (*ii).second;
	}
	
	//deletes for the list of positions
	for (WallsPositions::iterator it = _wallsPos.begin(); it != _wallsPos.end(); it++){
		delete *it;
	}
}

Model& MyResourceManager::getModelByName(std::string name){
	return *(_modelsMap[name]);
}
//TODO: Test the below code and use it
void MyResourceManager::setAModel(string fileName, string mapName){
	Model *m = new Model(fileName);
	_modelsMap[mapName] = m;
}

void MyResourceManager::addAWall(ofVec3f* p){
	_wallsPos.push_back(p);
}

ofVec3f& MyResourceManager::getWallPositionByIndex(int index){
	WallsPositions::iterator it = _wallsPos.begin();
	int i = 0;
	while( i < index ){
		it++;
	}
	return **(it);
	//return *(_wallsPos.at(index));	//si _wallsPos es vector
}

//TESTEJAR!!!!!
void MyResourceManager::updateWallsPositions(int index, ofVec3f* p){
	for (WallsPositions::iterator it = _wallsPos.begin(); it != _wallsPos.end(); it++){
		ofVec3f* p2=*it;
		p2->x = p2->x + p->x;
		p2->y = p2->y + p->y;
		p2->z = p2->z + p->z;
		*it = p2;					//PERILLOS!!!!!!!!!!!
	}
}

void MyResourceManager::deleteFirstWall(){
	_wallsPos.pop_front();
}

int MyResourceManager::numberOfWalls(){
	return _wallsPos.size();
}

void MyResourceManager::loadMapsFromXML(string filePath){
	ofxXmlSettings XML;
	_maps.clear();
	if(XML.loadFile(filePath)){
		int numWalls = XML.getNumTags("POSTURE");	//numero de parets	
	
		for (int i=0; i<numWalls; i++){
			XML.pushTag("POSTURE",i);
			_cols = XML.getValue("DIMENSIONS:COLS",0);
			_rows = XML.getValue("DIMENSIONS:ROWS",0);
			GridMap g;
			XML.pushTag("VALUES");
			for (int j=0; j < _cols*_rows; j++){
				int value = XML.getValue("PT",1,j);
				if (value == 1) value = 0;
				else			value = 1;
				g.push_back(value);
			}
			XML.popTag();
			_maps.push_back(g);
			XML.popTag();
		}
		std::cout << "Opening the wall postures file " << filePath << endl;
	} else{
		std::cout << "Failed opening the wall postures file " << filePath << ". Check it!" << endl;
	}
}

void MyResourceManager::createWallsFromMaps(ofVec3f playerPos){
	_walls.clear();
	for (int i=0; i<_maps.size(); i++){
		ofVec3f p = ofVec3f(0,0,20+i*50);
		Wall* w = new Wall(_rows, _cols, &this->getModelByName("cube"), p, playerPos, "yellow");
		w->setWallForm(_maps.at(i));
		_walls.push_back(w);
	}
}