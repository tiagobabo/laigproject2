#include "SceneLoader.h"

// Apontadores para os principais grupos
// declarados aqui como globais por conveni�ncia
// idealmente tudo seria inclu�do numa classe

//-------------------------------------------------------

Scene scene;
Node* root;
vector<Object*> objects;
vector<CompoundObject*> compoundobjects;

void endError(char* message)
{
	cout << message << endl;
	system("pause");
	exit(-1);
}

TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Fun��o de pesquisa de um n� filho por compara��o de um atributo (normalmente um id) com um valor de refer�ncia
// numa vers�o mais alto n�vel seria utilizada uma express�o XPath
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

/*
//-------------------------------------------------------

void processGraphNode(TiXmlElement *parent, int nivel)
// fun��o recursiva de processamento do grafo
// ao encontrar um n�, aplica as defini��es declaradas e itera sobre os seus descendentes
// nota: se um n� A, declarado em 'Nodes' aparecer instanciado uma vez no grafo com descendentes B e C
// e posteriormente o n� A for novamente instanciado, a rela��o de descend�ncia com B e C n�o � impl�cita.
// Neste modelo o n� � s� uma transforma��o geom�trica ou de atributos, n�o uma defini��o de um sub-grafo 
// com v�rios n�s que pode ser reinstanciado
{
	TiXmlElement *child=parent->FirstChildElement();

	// cria um prefixo de espa�os para indenta��o, apenas para visualiza��o
	int prefixLen=nivel*2;
	char prefix[100];
	memset(prefix,' ',prefixLen);
	prefix[prefixLen]=0;

	while (child)
	{
		if (strcmp(child->Value(),"Node")==0)
		{
			// � um n�
			printf("%s Nodo do tipo '%s' com id '%s'\n",prefix, child->Value(), child->Attribute("id"));
			// acede aos dados do n� e aplica materiais, texturas, transforma��es
			// para aceder ao n�, existiriam v�rias alternativas
			// aqui implementa-se uma pesquisa b�sica
			TiXmlElement *node=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

			if (node)
			{
				printf("%s     - Material id: '%s' \n", prefix, node->FirstChildElement("material")->Attribute("id"));
				printf("%s     - Texture id: '%s' \n", prefix, node->FirstChildElement("texture")->Attribute("id"));

				// repetir para outros detalhes do n�
			}


			// processa recursivamente os seus descendentes
			processGraphNode(child,nivel+1);
		}
		if (strcmp(child->Value(),"Leaf")==0)
		{
			// � folha
			printf("%s Folha do tipo '%s' com id '%s'\n",prefix, child->Value(), child->Attribute("id"));

			// acede aos dados declarados na sec��o Leaves
			TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

			if (leaf)
			{
				printf("%s     - tipo de folha: '%s' \n", prefix, leaf->Attribute("type"));
				// repetir para outros detalhes do n�
			}
			
			
			// e faz o render propriamente dito de acordo com o tipo de primitiva
			
		}

		child=child->NextSiblingElement();
	}

}
*/

void processGlobals(void)
{
	TiXmlElement* globalsElement=scene.sgxElement->FirstChildElement("globals");
		if (globalsElement)
		{
			cout <<  endl <<"::Processing globals::"<< endl<< endl;
			scene.root = globalsElement->Attribute("root");
			if (globalsElement->QueryIntAttribute("maxlights",&scene.maxlights)==TIXML_SUCCESS && 
				globalsElement->QueryIntAttribute("maxmaterials",&scene.maxmaterials)==TIXML_SUCCESS &&
				globalsElement->QueryIntAttribute("maxobjects",&scene.maxobjects)==TIXML_SUCCESS &&
				globalsElement->QueryIntAttribute("maxtextures",&scene.maxtextures)==TIXML_SUCCESS &&
				!scene.root.empty())
				cout << "root: " << scene.root << endl <<"maxlights: "<< scene.maxlights << endl <<"maxmaterials: "<< scene.maxmaterials << endl <<"maxobjects: " << scene.maxobjects << endl <<"maxtextures: " << scene.maxtextures  <<endl;
			else
				endError("Error parsing globals.");
		}
		else
			endError("Error globals not found.");
}

void process_translation(TiXmlElement* translateElement)
{
	// translate: exemplo para um n� com um par�metro que aglutina v�rios floats
		if (translateElement)
		{
			float x,y,z;
			if(translateElement->QueryFloatAttribute("x",&x)==TIXML_SUCCESS &&
				translateElement->QueryFloatAttribute("y",&y)==TIXML_SUCCESS &&
				translateElement->QueryFloatAttribute("z",&z)==TIXML_SUCCESS)
			{
					glTranslatef(x,y,z);
					cout << "ValX: " << x  << " |ValY: " << y << " |ValZ: " << z << endl;
			}
			else
				endError("Error parsing translation.");
		}
		else
			endError("Error translate not found.");	
}

void process_rotation(TiXmlElement* rotateElement)
{
	// translate: exemplo para um n� com um par�metro que aglutina v�rios floats
		if (rotateElement)
		{
			const char* axisl = rotateElement->Attribute("axis");
			if(axisl != NULL)
			{
				string axis;
				axis.append(axisl);
				if(axis.compare("x")==0 || axis.compare("y")==0 || axis.compare("z")==0)
				{

					float angle;
					if(rotateElement->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
					{
						//3 casos de rotacao
						if(axis.compare("x") == 0)
							glRotatef(angle, 1.0, 0.0,0.0);
						else if(axis.compare("y") == 0)
							glRotatef(angle, 0.0, 1.0,0.0);
						else
							glRotatef(angle, 0.0, 0.0,1.0);
						
						cout << "RotAxis: " << axis  << " |Ang: " << angle << endl;
					}
					else
						endError("Error parsing rotation.");
				}
				else
					endError("Error parsing rotation axis.");
			}
			else
				endError("Error parsing rotation axis.");
		}
		else
			endError("Error rotation not found.");	
}

void process_scale(TiXmlElement* scaleElement)
{
	if (scaleElement)
		{
			float x,y,z;
			if(scaleElement->QueryFloatAttribute("x",&x)==TIXML_SUCCESS &&
				scaleElement->QueryFloatAttribute("y",&y)==TIXML_SUCCESS &&
				scaleElement->QueryFloatAttribute("z",&z)==TIXML_SUCCESS)
			{
				glScalef(x,y,z);
				cout << "ScaleX: " << x  << " |ScaleY: " << y << " |ScaleZ: " << z << endl;
			}
			else
				endError("Error parsing scale.");
		}
		else
			endError("Error scale not found.");	
}

void processView(void)
{
	TiXmlElement* viewElement=scene.sgxElement->FirstChildElement("view");
		if (viewElement)
		{
			cout <<  endl <<"::Processing view::"<< endl << endl;
			if (viewElement->QueryFloatAttribute("near",&scene.near)==TIXML_SUCCESS && 
				viewElement->QueryFloatAttribute("far",&scene.far)==TIXML_SUCCESS &&
				viewElement->QueryFloatAttribute("axisscale",&scene.axisscale)==TIXML_SUCCESS)
				cout << "view atributes: " << scene.axisscale << endl;
			else
				endError("Error parsing view.");
		}
		else
			endError("Error view not found.");
		TiXmlElement* child = viewElement->FirstChildElement();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		while(child)
		{
			if(strcmp(child->Value(), "translation")==0)
				process_translation(child);
			else if(strcmp(child->Value(), "rotation")==0)
				process_rotation(child);
			else if(strcmp(child->Value(), "scale")==0)
				process_scale(child);
			else
				endError("Error parsing view: invalid child.");
			child=child->NextSiblingElement();
		}

		glGetFloatv(GL_MODELVIEW_MATRIX, &scene.m[0][0]);
		cout << "ola";
}

void processIllumination_ambient(TiXmlElement* ambient)
{
	float r,g,b,a;
	if(ambient->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene.ambient[0] = r;
		scene.ambient[1] = g;
		scene.ambient[2] = b;
		scene.ambient[3] = a;

		cout << "ambient: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		endError("Error parsing illumination: ambient not found or invalid.");
}

void processIllumination_background(TiXmlElement* background)
{
	float r,g,b,a;
	if(background->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene.background[0] = r;
		scene.background[1] = g;
		scene.background[2] = b;
		scene.background[3] = a;
		cout << "background: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		endError("Error parsing illumination: background not found or invalid.");
}

void processLight_position(TiXmlElement* position)
{
	float x,y,z,w;
	if(	position->QueryFloatAttribute("x",&x)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("y",&y)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("z",&z)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("w",&w)==TIXML_SUCCESS)
	{
		scene.lights.back()->position[0] = x;
		scene.lights.back()->position[1] = y;
		scene.lights.back()->position[2] = z;
		scene.lights.back()->position[3] = w;
		cout << "position: " << x << " " << y << " " << z << " " << w << endl;
	}
	else
		endError("Error parsing Light: position not found or invalid.");
}

void processLight_type(TiXmlElement* type, int ntype)
{
	float r,g,b,a;
	if(	type->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		switch(ntype){
		case 0:
			{
				scene.lights.back()->ambient[0] = r;
				scene.lights.back()->ambient[1] = g;
				scene.lights.back()->ambient[2] = b;
				scene.lights.back()->ambient[3] = a;
				cout << "Light_Ambient: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		case 1:
			{
				scene.lights.back()->diffuse[0] = r;
				scene.lights.back()->diffuse[1] = g;
				scene.lights.back()->diffuse[2] = b;
				scene.lights.back()->diffuse[3] = a;
				cout << "Light_diffuse: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		case 2:
			{
				scene.lights.back()->specular[0] = r;
				scene.lights.back()->specular[1] = g;
				scene.lights.back()->specular[2] = b;
				scene.lights.back()->specular[3] = a;
				cout << "Light_specular: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		default:
			break;
		}
	}
	else
		endError("Error parsing Light: argument not found or invalid.");
}

void processLights_light(TiXmlElement* light)
{
	const char* id = light->Attribute("id");
	const char* enabled = light->Attribute("enabled");

	if(id != NULL && enabled != NULL && (strcmp(enabled, "1") == 0 || strcmp(enabled, "0")))
	{
		string id2 = light->Attribute("id");
		vector<Light*>::iterator it;
		for(it=scene.lights.begin() ; it < scene.lights.end(); it++)
		{
			if((*it)->id.compare(id2) == 0)
			{
				char temp[255];
				sprintf(temp, "Error parsing lights: id %s already exists.", id2.c_str());
				endError(temp);
			}
		}
		cout << "Parsing light id = " << id << endl;


		if(strcmp(enabled, "1") == 0)
		{
			Light* l = new Light(id2, true);
			scene.lights.push_back(l);
		}
		else
		{
			Light* l = new Light(id2, false);
			scene.lights.push_back(l);
		}

		TiXmlElement* child = light->FirstChildElement();
		 
		while(child)
		{
			if(strcmp(child->Value(), "position")==0)
				processLight_position(child);
			else if(strcmp(child->Value(), "ambient")==0)
				processLight_type(child,0);
			else if(strcmp(child->Value(), "diffuse")==0)
				processLight_type(child,1);
			else if(strcmp(child->Value(), "specular")==0)
				processLight_type(child,2);
			else
				endError("Error parsing light: invalid child.");
			child = child->NextSiblingElement();
		}

	}
	else
		endError("Error parsing light: id or enabled not found or invalid.");
}

void processIllumination_lights(TiXmlElement* lights)
{
	int nChilds = 0;
	TiXmlElement* child = lights->FirstChildElement();
	while(child)
	{
		if(nChilds == scene.maxlights)
			endError("Exceeded number of lights permitted.");
		else{
			if(strcmp(child->Value(), "light")==0){
				processLights_light(child);
			}
			else
				endError("Error parsing lights: invalid child.");
			
			child = child->NextSiblingElement();
			nChilds++;
		}
	}
}

void processIllumination(void)
{
	TiXmlElement* illuminationElement=scene.sgxElement->FirstChildElement("illumination");
	if (illuminationElement)
		{
			cout <<  endl <<"::Processing Illumination:: " << endl<<endl;
			const char* doublesided = illuminationElement->Attribute("doublesided");
			const char* local = illuminationElement->Attribute("local");
			if(doublesided != NULL && local != NULL && (strcmp(doublesided, "1") == 0 || strcmp(doublesided, "0") == 0) && (strcmp(local, "1") == 0 || strcmp(local, "0") == 0))
			{
				if(strcmp(doublesided, "1") == 0) 
					scene.doublesided = true;
				else
					scene.doublesided = false;

				if(strcmp(local, "1") == 0) 
					scene.local = true;
				else
					scene.local = false;

				cout << "illumination atributes: " << scene.doublesided << " " << scene.local << endl; 

				TiXmlElement* child = illuminationElement->FirstChildElement();

				while(child)
				{
					if(strcmp(child->Value(), "ambient")==0)
						processIllumination_ambient(child);
					else if(strcmp(child->Value(), "background")==0)
						processIllumination_background(child);
					else if(strcmp(child->Value(), "lights")==0)
						processIllumination_lights(child);
					else
						endError("Error parsing illumination: invalid child.");
					child=child->NextSiblingElement();
				}

			}
			else
				endError("Error parsing illumination: doublesided or local not found or invalid.");
		}
		else
			endError("Error illumination not found.");
}


void processTextures_texture(TiXmlElement* texture)
{
	const char* id = texture->Attribute("id");
	const char* file = texture->Attribute("file");
	float s,t;
	if(	texture->QueryFloatAttribute("length_s",&s)==TIXML_SUCCESS &&
		texture->QueryFloatAttribute("length_t",&t)==TIXML_SUCCESS && id != NULL && file != NULL)
	{
		string id2 = texture->Attribute("id");
		vector<Texture*>::iterator it;
		for(it=scene.textures.begin() ; it < scene.textures.end(); it++)
		{
			if((*it)->id.compare(id2) == 0)
			{
				char temp[255];
				sprintf(temp, "Error parsing textures: id %s already exists", id2.c_str());
				endError(temp);
			}
		}
		Texture* t1 = new Texture(id,file, s,t);
		scene.textures.push_back(t1);
		cout << "Texture id: "<< scene.textures.back()->id << " | file: " << scene.textures.back()->file << " | length_s: " << scene.textures.back()->length_s << " | length_t: " << scene.textures.back()->length_t << endl;
	}
	else
		endError("Error parsing texture: argument not found or invalid.");
}


void processTexture(void)
{
	int nChilds = 0;
	TiXmlElement* texturesElement=scene.sgxElement->FirstChildElement("textures");
	if (texturesElement)
		{
			cout << endl << "::Processing Texture:: " << endl<< endl;
			TiXmlElement* child = texturesElement->FirstChildElement();
			while(child)
			{
				if(nChilds == scene.maxtextures)
					endError("Exceeded number of textures permitted.");
				else{
					processTextures_texture(child);
					child=child->NextSiblingElement();
					nChilds++;
				}
			}
		}
		else
			endError("Error textures not found.");
}

void processMaterial_type(TiXmlElement* type, int ntype)
{
	float r,g,b,a;
	if(	type->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
		type->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		switch(ntype){
		case 0:
			{
				scene.materials.back()->emission[0] = r;
				scene.materials.back()->emission[1] = g;
				scene.materials.back()->emission[2] = b;
				scene.materials.back()->emission[3] = a;
				cout << "Material_emission: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		case 1: 
			{
				scene.materials.back()->ambient[0] = r;
				scene.materials.back()->ambient[1] = g;
				scene.materials.back()->ambient[2] = b;
				scene.materials.back()->ambient[3] = a;
				cout << "Material_ambient: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		case 2:
			{
				scene.materials.back()->diffuse[0] = r;
				scene.materials.back()->diffuse[1] = g;
				scene.materials.back()->diffuse[2] = b;
				scene.materials.back()->diffuse[3] = a;
				cout << "Material_diffuse: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		case 3:
			{
				scene.materials.back()->specular[0] = r;
				scene.materials.back()->specular[1] = g;
				scene.materials.back()->specular[2] = b;
				scene.materials.back()->specular[3] = a;
				cout << "Material_specular: " << r << " " << g << " " << b << " " << a << endl;
			}
			break;
		}
	}
	else
		endError("Error parsing Material: argument not found or invalid.");
}

void processMaterial_shininess(TiXmlElement* texture)
{
	float s;
	if(	texture->QueryFloatAttribute("value",&s)==TIXML_SUCCESS)
	{
		scene.materials.back()->shininess = s;
		cout << "Material shininess: "<< scene.materials.back()->shininess << endl;
	}
	else
		endError("Error parsing Material: shininess not found or invalid.");
}

void processMaterials_material(TiXmlElement* material)
{
	const char* id = material->Attribute("id");
	if(id != NULL)
	{
		string id2 = material->Attribute("id");
		vector<Material*>::iterator it;
		for(it=scene.materials.begin() ; it < scene.materials.end(); it++)
		{
			if((*it)->id.compare(id2) == 0)
			{
				char temp[255];
				sprintf(temp, "Error parsing materials: id %s already exists", id2);
				endError(temp);
			}
		}
		cout << "Parsing Material id = " << id << endl;
		Material* m = new Material(id);
		scene.materials.push_back(m);

		TiXmlElement* child = material->FirstChildElement();
		while(child)
		{
			if(strcmp(child->Value(), "emission")==0)
				processMaterial_type(child, 0);
			else if(strcmp(child->Value(), "ambient")==0)
				processMaterial_type(child,1);
			else if(strcmp(child->Value(), "diffuse")==0)
				processMaterial_type(child,2);
			else if(strcmp(child->Value(), "specular")==0)
				processMaterial_type(child,3);
			else if(strcmp(child->Value(), "shininess")==0)
				processMaterial_shininess(child);
			else
				endError("Error parsing Material: invalid child.");
			child=child->NextSiblingElement();
		}
	}
	else
		endError("Error parsing Material: id not found or invalid.");
}

void processMaterial(void)
{
	int nChilds = 0;
	TiXmlElement* materialsElement=scene.sgxElement->FirstChildElement("materials");
	if (materialsElement)
		{
			cout << endl <<"::Processing Material:: " << endl << endl;
			TiXmlElement* child = materialsElement->FirstChildElement();
			while(child)
			{	
				if(nChilds == scene.maxmaterials)
					endError("Exceeded number of materials permitted.");
				else
				{
					processMaterials_material(child);
					child=child->NextSiblingElement();
					nChilds++;
				}
			}
		}
		else
			endError("Error materials not found.");
}

void processObject_transformations(TiXmlElement* transformations, string type)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	TiXmlElement* child = transformations->FirstChildElement();
	while(child)
	{
		if(strcmp(child->Value(),"translation")==0)
			process_translation(child);
		else if(strcmp(child->Value(),"rotation")==0)
			process_rotation(child);
		else if(strcmp(child->Value(),"scale")==0)
			process_scale(child);
		else
			endError("Error parsing Object Transformation: invalid child");

		child=child->NextSiblingElement();
	}
	if(type == "compound")
		glGetFloatv(GL_MODELVIEW_MATRIX, &compoundobjects.back()->transformations[0][0]);
	else
		glGetFloatv(GL_MODELVIEW_MATRIX, &objects.back()->transformations[0][0]);
}

void processObject_type(TiXmlElement* type, int ntype, string typeObject)
{
	const char* id = type->Attribute("id");
	if(id != NULL)
	{

		switch(ntype){
		case 0:
			if(typeObject == "compound")
				compoundobjects.back()->setMaterial(id);
			else
				objects.back()->setMaterial(id);
			cout<< "ID MATERIAL: " << id << endl;
			break;
		case 1:
			if(typeObject == "compound")
				compoundobjects.back()->setTexture(id);
			else
				objects.back()->setTexture(id);
			cout<< "ID TEXTURE: " << id << endl;
			break;
		default:
			break;
		}
	}
	else
		endError("Error parsing id on argument of Object.");
}

void processObject_rectangle(TiXmlElement* rectangle, string typeObject, string id)
{
	float x1,x2,y1,y2;
	if(	rectangle->QueryFloatAttribute("x1",&x1)==TIXML_SUCCESS &&
		rectangle->QueryFloatAttribute("x2",&x2)==TIXML_SUCCESS &&
		rectangle->QueryFloatAttribute("y1",&y1)==TIXML_SUCCESS &&
		rectangle->QueryFloatAttribute("y2",&y2)==TIXML_SUCCESS)
	{
		Rectangle* r = new Rectangle(id, typeObject, x1, y1, x2, y2);
		objects.push_back(r);
		cout << "Rectangle:"  << endl << "x1: "<< x1 << " x2: "<< x2 << " y1: " << y1 << " y2: "<< y2<< endl;
	}
	else
		endError("Error parsing Object Rectangle: argument not found or invalid.");
}

void processObject_triangle(TiXmlElement* triangle, string typeObject, string id)
{
	float x1,x2, x3,y1,y2, y3, z1,z2,z3;
	if(	triangle->QueryFloatAttribute("x1",&x1)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("y1",&y1)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("z1",&z1)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("x2",&x2)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("y2",&y2)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("z2",&z2)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("x3",&x3)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("y3",&y3)==TIXML_SUCCESS &&
		triangle->QueryFloatAttribute("z3",&z3)==TIXML_SUCCESS)
	{
		Triangle* t = new Triangle(id, typeObject, x1, x2, x3, y1, y2, y3, z1, z2, z3);
		objects.push_back(t);
		cout << "Triangle: " << endl <<"x1: "<< x1 << " y1: "<< y1 << " z1: " << z1<< endl << "x2: "<< x2<< " y2: "<< y2 << " z2: " << z2<< endl << "x3: "<< x3<< " y3: "<< y3 << " z3: " << z3<< endl;
	}
	else
		endError("Error parsing Object Triangle: argument not found or invalid.");
}

void processObject_cylinder(TiXmlElement* cylinder, string typeObject, string id)
{
	float base,top,height;
	int slices, stacks;
	if(	cylinder->QueryFloatAttribute("base",&base)==TIXML_SUCCESS &&
		cylinder->QueryFloatAttribute("top",&top)==TIXML_SUCCESS &&
		cylinder->QueryFloatAttribute("height",&height)==TIXML_SUCCESS &&
		cylinder->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS &&
		cylinder->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
	{
		Cylinder* c = new Cylinder(id, typeObject, base, top, height, slices, stacks);
		objects.push_back(c);
		cout << "Cylinder: " << endl <<"base: "<< base << " top: "<< top << " height: " << height<< endl << "slices: "<< slices<< " stacks: "<< stacks << endl;
	}
	else
		endError("Error parsing Object Cylinder: argument not found or invalid.");
}

void processObject_sphere(TiXmlElement* sphere, string typeObject, string id)
{
	float radius;
	int slices, stacks;
	if(	sphere->QueryFloatAttribute("radius",&radius)==TIXML_SUCCESS &&
		sphere->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS &&
		sphere->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
	{
		Sphere* s = new Sphere(id, typeObject, radius, slices, stacks);
		objects.push_back(s);
		cout << "Sphere: " << endl <<"radius: "<< radius << " slices: "<< slices<< " stacks: "<< stacks << endl;
	}
	else
		endError("Error parsing Object Sphere: argument not found or invalid.");
}

void processObject_disk(TiXmlElement* disk, string typeObject, string id)
{
	float inner, outer;
	int slices, loops;
	if(	disk->QueryFloatAttribute("inner",&inner)==TIXML_SUCCESS &&
		disk->QueryFloatAttribute("outer",&outer)==TIXML_SUCCESS &&
		disk->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS &&
		disk->QueryIntAttribute("loops",&loops)==TIXML_SUCCESS)
	{
		Disk* d = new Disk(id, typeObject, inner, outer, slices, loops);
		objects.push_back(d);
		cout << "Disk: " << endl <<"inner: "<< inner << " outer: "<< outer<< " slices: "<< slices << " loops: "<< loops << endl;
	}
	else
		endError("Error parsing Object Disk: argument not found or invalid.");
}

void processObject_geometry(TiXmlElement* object, string typeObject, string id)
{
	const char* type = object->Attribute("type");
	if(type != NULL)
	{
		if(strcmp(type,"rectangle")==0)
			processObject_rectangle(object, typeObject, id);
		else if(strcmp(type,"triangle")==0)
			processObject_triangle(object, typeObject, id);
		else if(strcmp(type,"cylinder")==0)
			processObject_cylinder(object, typeObject, id);
		else if(strcmp(type,"sphere")==0)
			processObject_sphere(object, typeObject, id);
		else if(strcmp(type,"disk")==0)
			processObject_disk(object, typeObject, id);
		else
			endError("Error parsing Object: invalid type.");
	}
	else
		endError("Error parsing Object type.");
}

void processChildren_objRef(TiXmlElement* children)
{
	const char* id = children->Attribute("id");
	string id2;
	id2.append(id);
	compoundobjects.back()->addId(id2);
	cout << "Objectref: " << id << endl;
}

void processObject_children(TiXmlElement* object)
{
	TiXmlElement* child = object->FirstChildElement();
	while(child)
	{
		if(strcmp(child->Value(),"objectref")==0)
			processChildren_objRef(child);

		child=child->NextSiblingElement();
	}
}

void processObject_simple(TiXmlElement* object, string type)
{
	if(strcmp(object->Value(), "transformations")==0)
		processObject_transformations(object, type);
	else if(strcmp(object->Value(), "material")==0)
		processObject_type(object,0, type);
	else if(strcmp(object->Value(), "texture")==0)
		processObject_type(object,1, type);
}

void processObject_compound(TiXmlElement* object, string type)
{
	if(strcmp(object->Value(), "transformations")==0)
		processObject_transformations(object, type);
	else if(strcmp(object->Value(), "material")==0)
		processObject_type(object,0, type);
	else if(strcmp(object->Value(), "texture")==0)
		processObject_type(object,1, type);
	else if(strcmp(object->Value(), "children")==0)
		processObject_children(object);
}

void checkIdObject(vector<Object*> vector1, vector<CompoundObject*> vector2, string id)
{
	vector<Object*>::iterator it;
	for(it=vector1.begin() ; it < vector1.end(); it++)
	{
		if((*it)->getID().compare(id) == 0)
		{
			char temp[255];
			sprintf(temp,"Error parsing Object: id %s already exists", id.c_str());
			endError(temp);
		}
	}
	vector<CompoundObject*>::iterator it2;
	for(it2=vector2.begin() ; it2 < vector2.end(); it2++)
	{
		if((*it2)->getID().compare(id) == 0)
		{
			char temp[255];
			sprintf(temp,"Error parsing Object: id %s already exists", id.c_str());
			endError(temp);
		}
	}
}

void processObjects_object(TiXmlElement* object)
{
	const char* id = object->Attribute("id");
	const char* type = object->Attribute("type");
	if(id != NULL && ((strcmp(type, "compound") == 0) || (strcmp(type, "simple") == 0)))
	{
		string id2 = object->Attribute("id");
		
		cout << "Parsing Object id = " << id << endl;
		cout << "Parsing Object type = " << type << endl;
		checkIdObject(objects, compoundobjects, id);

		// Process simple object [first geometry, then rest]
		TiXmlElement* child = object->FirstChildElement();
		int geo = 0;
		if(strcmp(type, "simple") == 0)
		{
			while(child)
			{
				if(strcmp(child->Value(),"geometry") == 0 && !geo)
				{
					processObject_geometry(child, type, id);
					geo = 1;
				}
				else if(strcmp(child->Value(),"geometry") == 0 && geo)
					endError("Error parsing object: object has more than one geometry");
				child=child->NextSiblingElement();
			}
			
			child = object->FirstChildElement();
			while(child)
			{
				processObject_simple(child, type);
				child=child->NextSiblingElement();
			}

		}

		child = object->FirstChildElement();
		if(strcmp(type, "compound")==0)
		{
			CompoundObject* c = new CompoundObject(id, type);
			string aux;
			aux.append(id);
			if(id == scene.root)
				root = c;
			compoundobjects.push_back(c);
			while(child)
			{
				processObject_compound(child, type);
				child=child->NextSiblingElement();
			}
		}
	}
	else
		endError("Error parsing Object: id not found or invalid.");
}

void processObjects(void)
{

	int nChilds = 0;
	TiXmlElement* objectsElement=scene.sgxElement->FirstChildElement("objects");
	if (objectsElement)
		{
			cout << endl << "::Processing Objects:: " << endl<< endl;
			TiXmlElement* child = objectsElement->FirstChildElement();
			while(child)
			{
				if(nChilds == scene.maxobjects)
					endError("Exceeded number of objects permitted.");
				else{
					processObjects_object(child);
					child=child->NextSiblingElement();
					nChilds++;
				}
			}
		}
		else
			endError("Error objects not found.");
}

void createTree(Node* node, int i)
{
	vector<Node*>::iterator it;
	for(it=node->nodes.begin() ; it < node->nodes.end(); it++)
	{
		if((*it)->getType() == "compound")
		{
			int w;
			for(w =0; w < i; w++)
				cout << " ";
			w++;
			cout << (*it)->getID() << " " << (*it) << endl;
			createTree((*it),w);
		}
		else
		{
			int w;
			for(w =0; w < i; w++)
				cout << " ";
			cout << "OS: " << (*it)->getID() << " " << (*it) << endl;
		}
	}
}

void mapTextures(Node* node)
{
	vector<Node*>::iterator it;
	for(it=node->nodes.begin() ; it < node->nodes.end(); it++)
	{
		if((*it)->getTextureId() != "null")
		{
			vector<Texture*>::iterator itM;
			for(itM = scene.textures.begin(); itM < scene.textures.end(); itM++)
				if((*itM)->id == (*it)->getTextureId())
					(*it)->texture = *itM;

			if((*it)->texture == NULL)
				if((*it)->getTextureId() != "clear"){
					char temp[255];
					sprintf(temp,"Error parsing texture: id %s don't exist", (*it)->getTextureId().c_str());
					endError(temp);
				}	
		}
		else
			(*it)->texture = node->texture;

		if((*it)->getType() == "compound") mapTextures(*it);
	}
}

void mapMaterials(Node* node)
{
	vector<Node*>::iterator it;
	for(it=node->nodes.begin() ; it < node->nodes.end(); it++)
	{
		if((*it)->getMaterialId() != "null")
		{
			vector<Material*>::iterator itM;
			for(itM = scene.materials.begin(); itM < scene.materials.end(); itM++)
			{
				if((*itM)->id == (*it)->getMaterialId())
					(*it)->material = *itM;
			}
		}
		else
			(*it)->material = node->material;

		if((*it)->getType() == "compound") mapMaterials(*it);
	}
}

void mapTransformations(Node* node, int root)
{
	glMatrixMode(GL_MODELVIEW);
	vector<Node*>::iterator it;
	for(it=node->nodes.begin() ; it < node->nodes.end(); it++)
	{
		if(root == 1)
			glLoadIdentity();
		glPushMatrix();
			glMultMatrixf(&node->transformations[0][0]);
			glMultMatrixf(&(*it)->transformations[0][0]);
			glGetFloatv(GL_MODELVIEW_MATRIX, &(*it)->transformations[0][0]);
		glPopMatrix();
		mapTransformations(*it, 0);
	}
	if(node->getType() == "simple")
	{
		glPushMatrix();
			glMultMatrixf(&node->transformations[0][0]);
			glGetFloatv(GL_MODELVIEW_MATRIX, &node->transformations[0][0]);
		glPopMatrix();
	}
}

void mapCompoundObjects(Node* node)
{
	for(int i = 0; i < node->ids.size() ; i++)
	{
		string id = node->ids.at(i);
		for(int j = 0; j < compoundobjects.size(); j++)
			if(compoundobjects.at(j)->getID() == id)
			{
				Node* n = compoundobjects.at(j)->clone();
				node->nodes.push_back(n);
				mapCompoundObjects(n);
			}
		vector<Object*>::iterator it;
		for(it=objects.begin() ; it < objects.end(); it++)
			if((*it)->getID() == id)
			{
				Node* temp = (*it)->clone();
				node->nodes.push_back(temp);
			}
	}
}

Node* loadScene(Scene* s)
{
	// Read string from file
	TiXmlDocument doc( "bridge.xml" );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		char temp[255];
		sprintf(temp, "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		endError(temp);
	}

	scene.sgxElement = doc.FirstChildElement( "sgx" ); 
	scene.texturesElement = doc.FirstChildElement("textures");
	scene.materialsElement = doc.FirstChildElement("materials");
	scene.lightsElement = doc.FirstChildElement("lights");
	scene.objectsElement = doc.FirstChildElement("objects");

	// Inicializa��o
	// Um exemplo de um conjunto de n�s bem conhecidos e obrigat�rios

	if (scene.sgxElement == NULL)
		printf("Bloco sgx n�o encontrado\n");
	else
	{
		printf("Processing sgx:\n");
		// frustum: exemplo para n� com par�metros individuais
		processGlobals();
		processView();
		processIllumination();
		processTexture();
		processMaterial();
		processObjects();
		if(root == NULL){
			cout << "Root not found."<< endl;
			system("pause");
			exit(-1);
		}
		mapCompoundObjects(root);
		createTree(root, 0);

		mapMaterials(root);
		mapTextures(root);
		mapTransformations(root, 1);
	}
	*s = scene;
	return root;
}