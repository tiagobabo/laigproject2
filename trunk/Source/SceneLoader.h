#include "..\tinyxml.h"

#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;


class Scene
{
public:
	TiXmlElement* sgxElement; 
	TiXmlElement* texturesElement;
	TiXmlElement* materialsElement;
	TiXmlElement* lightsElement;
	TiXmlElement* objectsElement;

	//globals
	int maxlights;
	int maxtextures; 
	int maxmaterials; 
	int maxobjects;
	string root;

	//view
	float near;
	float far;
	float axisscale;
	float valX;
	float valY;
	float valZ;
	const char* axis;
	float angle;
	float scaleX;
	float scaleY;
	float scaleZ;


	Scene(){
		sgxElement=NULL; 
		texturesElement=NULL;
		materialsElement=NULL;
		lightsElement=NULL;
		objectsElement=NULL;
	}


};

void loadScene(void);