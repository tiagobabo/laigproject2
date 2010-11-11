#include "SceneLoader.h"

void loadMaterial(Material* m)
{
	// define caracteristicas de cor do material do plano e da caixa
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialfv(GL_FRONT, GL_SHININESS, &m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  m->specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   m->diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   m->ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION,   m->emission);
}

//constructores das classes
Node::Node(string id, string type)
{
	this->id = id;
	this->type = type;
	this->texture = NULL;
	this->material = NULL;
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
	this->textureid = t;
}

void Node::setMaterial(string m)
{
	this->materialid = m;
}

string Node::getTextureId()
{
	return this->textureid;
}

string Node::getMaterialId()
{
	return this->materialid;
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
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	cout << "DESENHO TRIANGLE" << endl;
	glPopMatrix();
}

void Rectangle::draw()
{
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	glBegin(GL_POLYGON);
	glNormal3d(0.0,0.0,1.0);
		glVertex3d(this->x1, this->y1, 0.0);
		glVertex3d(this->x2, this->y1, 0.0);
		glVertex3d(this->x2, this->y2, 0.0);
		glVertex3d(this->x1, this->y2, 0.0);
	glEnd();
	glPopMatrix();
}
void Cylinder::draw()
{
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluCylinder(glQ2, this->base, this->top, this->height, this->slices, this->stacks);
	glPopMatrix();
}

void Disk::draw()
{
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluDisk(glQ2, this->inner, this->outer, this->slices, this->loops);
	glPopMatrix();
}
void Sphere::draw()
{
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	gluSphere(glQ2, this->radius, this->slices, this->stacks);
	glPopMatrix();
}

