#include <cstdlib>
#include <GL\glui.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>

#include "..\tinyxml.h"

using namespace std;

class Light
{
public:
	string id;
	bool enabled;
	float position[4];
	float ambient[4];
	float specular[4];
	float diffuse[4];

	Light(string id, bool enabled)
	{
		this->id = id;
		this->enabled = enabled;
		for(int i = 0; i < 4; i++)
		{
			this->position[i] = 0;
			this->ambient[i] = 0;
			this->specular[i] = 0;
			this->diffuse[i] = 0;
		}
	}
};

class Texture
{
public:
	string id;
	string file;
	float length_s;
	float length_t;
	Texture(string id, string file, float length_s, float length_t)
	{
		this->id = id;
		this->file = file;
		this->length_s = length_s;
		this->length_t = length_t;
	}
};

class Material
{
public:
	string id;
	float emission[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;

	Material(string id)
	{
		this->id = id;
		shininess = 0;
		for(int i = 0; i < 4; i++)
		{
			this->emission[i] = 0;
			this->ambient[i] = 0;
			this->specular[i] = 0;
			this->diffuse[i] = 0;
		}
	}
};

class Scene
{
public:
	TiXmlElement* sgxElement; 
	TiXmlElement* texturesElement;
	TiXmlElement* materialsElement;
	TiXmlElement* lightsElement;
	TiXmlElement* objectsElement;
	TiXmlElement* illuminationElement;

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

	//matriix de transformacao inicial
	float m[4][4];

	//lights
	bool doublesided, local;
	float ambient[4];
	float background[4];

	vector<Light> lights;
	vector<Texture> textures;
	vector<Material> materials;

	Scene(){
		sgxElement=NULL; 
		texturesElement=NULL;
		materialsElement=NULL;
		lightsElement=NULL;
		objectsElement=NULL;
		illuminationElement=NULL;
	}


};

void loadScene(void);

class Node
{
private:
	string id, type;
public:
	Node(string id, string type);
	//Node virtual hasChild();
};

class Object : public Node
{
private:
	
public:
	string teste;
	Object(string id, string type, string teste);
	//void virtual Draw();
	//Node hasChild();
};
 
class CompoundObject : public Node
{
private:
	vector<Node> nodes;
	int bla;
public:
	CompoundObject(string id, string type, int bla);
	//Node hasChild();
};