#pragma once
#include "ofxAssimpModelLoader.h"

class Model
{
public:
	Model(string fileName);
	ofTexture&	getTexture();
	ofMaterial&	getMaterial();
	ofMesh&		getMesh();
	void textureBinding(bool isBinded);
	void materialBeginEnd(bool isShown);
	void drawFacesMesh();
	void drawModel();

private:
	ofxAssimpModelLoader _basicCube;
	ofMesh _mesh;
	ofMaterial _material;
	ofTexture _tex;
};

