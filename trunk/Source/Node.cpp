#include "SceneLoader.h"

//constructores das classes
Node::Node(string id, string type)
{
	this->id = id;
	this->type = type;
}

Object::Object(string id, string type, string teste):Node(id,type)
{
	this->teste = teste;
}

CompoundObject::CompoundObject(string id, string type, int teste):Node(id,type)
{
	this->bla = teste;
}

Node* CompoundObject::hasChild()
{
	if(!this->nodes.empty())
	{
		Node* obj = this->nodes.back();
		nodes.pop_back();
		return obj;
	}
	return NULL;
}

Node* Object::hasChild()
{
	return NULL;
}

void CompoundObject::addNode(Node* node)
{
	this->nodes.push_back(node);
}

void Triangle::draw()
{
	cout << "DESENHO" << endl;
}

