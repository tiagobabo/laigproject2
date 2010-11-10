#include "SceneLoader.h"

//constructores das classes
Node::Node(string id, string type)
{
	this->id = id;
	this->type = type;
	this->texture;
	this->material;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, &transformations[0][0]);
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

void Node::setTexture(string t)
{
	this->texture = t;
}

void Node::setMaterial(string m)
{
	this->material = m;
}

string Node::getTexture()
{
	return this->texture;
}

string Node::getMaterial()
{
	return this->material;
}

void Node::setId(string id)
{
	this->id = id;
}

void CompoundObject::addNode(Node* node)
{
	this->nodes.push_back(node);
}

void CompoundObject::addId(string node)
{
	this->ids.push_back(node);
}


void CompoundObject::draw()
{
	vector<Node*>::iterator it;
	for(it=nodes.begin() ; it < nodes.end(); it++)
		(*it)->draw();
}

void Triangle::draw()
{
	cout << "DESENHO TRIANGLE" << endl;
}

void Rectangle::draw()
{
	glBegin(GL_POLYGON);
	glNormal3d(0.0,0.0,1.0);
		glVertex3d(this->x1, this->y1, 0.0);
		glVertex3d(this->x2, this->y1, 0.0);
		glVertex3d(this->x2, this->y2, 0.0);
		glVertex3d(this->x1, this->y2, 0.0);
	glEnd();
}
void Cylinder::draw()
{
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluCylinder(glQ2, this->base, this->top, this->height, this->slices, this->stacks);
}

void Disk::draw()
{
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluDisk(glQ2, this->inner, this->outer, this->slices, this->loops);
}
void Sphere::draw()
{
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluSphere(glQ2, this->radius, this->slices, this->stacks);
}
