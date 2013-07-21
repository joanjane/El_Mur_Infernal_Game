#include "Model.h"


Model::Model(string fileName)
{
	_basicCube.loadModel( fileName, true); //load the model with assimp
	
	_mesh = _basicCube.getMesh(0);					//copying the mesh, texture and material from model
   	_material = _basicCube.getMaterialForMesh(0);
    _tex = _basicCube.getTextureForMesh(0);

}


ofTexture& Model::getTexture(){
	return _tex;
}

ofMaterial& Model::getMaterial(){
	return _material;
}

ofMesh& Model::getMesh(){
	return _mesh;
}

void Model::textureBinding(bool isBinded){
	if(isBinded)	getTexture().bind();
	else			getTexture().unbind();
}
void Model::materialBeginEnd(bool isShown){
	if(isShown)	getMaterial().begin();
	else		getMaterial().end();
}

void Model::drawFacesMesh(){
	getMesh().drawFaces();
}

void Model::drawModel(){
	//_basicCube.getTextureForMesh().bind()
	//_tex.bind();
	//_material.begin();
	//_mesh.drawFaces();
	_mesh.draw();
	//_material.end();
	//_tex.unbind();
}
		