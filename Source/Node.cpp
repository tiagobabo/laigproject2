#include "SceneLoader.h"

//constructores das classes
Node::Node(string id, string type)
{
	this->id = id;
	this->type = type;
}

Object::Object(string id, string type):Node(id,type) {}

CompoundObject::CompoundObject(string id, string type):Node(id,type) {}

Triangle::Triangle(string id, string type, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3)
	:Object(id, type)
{
	this->x1= x1;
	this->x2= x2;
	this->x3= x3;
	this->y1= y1;
	this->y2= y2;
	this->y3= y3;
	this->z1= z1;
	this->z2= z2;
	this->z3= z3;
}

Rectangle::Rectangle(string id, string type, float x1, float y1, float x2, float y2)
	:Object(id, type)
{
	this->x1=x1;
	this->y1=y1;
	this->x2=x2;
	this->y2=y2;
}

Cylinder::Cylinder(string id, string type, float base, float top, float height, float slices, float stacks)
	:Object(id,type)
{
	this->base = base;
	this->top = top;
	this-> height = height;
	this->slices = slices;
	this->stacks = stacks;
}

Disk::Disk(string id, string type, float inner, float outer, float slices, float loops)
	:Object(id, type)
{
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;
}

Sphere::Sphere(string id, string type, float radius, float slices, float stacks)
	:Object(id, type)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
}

//Fim da construção das classes

//Funções partilhadas da classe Node

void Node::setTexture(Texture* t)
{
	this->texture = t;
}

void Node::setMaterial(Material* m)
{
	this->material = m;
}

void Node::setTransformations(float t[4][4])
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			this->transformations[i][j] = t[i][j];
}

Texture* Node::getTexture()
{
	return this->texture;
}

Material* Node::getMaterial()
{
	return this->material;
}

/*float** Node::getTransformations()
{
	return this->transformations;
}*/

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

void CompoundObject::addNode(Node* node)
{
	this->nodes.push_back(node);
}


Node* Object::hasChild()
{
	return NULL;
}


void Triangle::draw()
{
	cout << "DESENHO TRIANGLE" << endl;
}

void Rectangle::draw()
{
	cout << "DESENHA RECTANGULO" << endl;
}
void Cylinder::draw()
{
	cout << "DESENHO CILINDRO" << endl;
}

void Disk::draw()
{
	cout << "DESENHA DISCO" << endl;
}
void Sphere::draw()
{
	cout << "DESENHO ESFERA" << endl;
}
