#include "SceneLoader.h"

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