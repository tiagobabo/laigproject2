#include "SceneLoader.h"

// Apontadores para os principais grupos
// declarados aqui como globais por conveniência
// idealmente tudo seria incluído numa classe

//-------------------------------------------------------

Scene* scene = new Scene();

TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Função de pesquisa de um nó filho por comparação de um atributo (normalmente um id) com um valor de referência
// numa versão mais alto nível seria utilizada uma expressão XPath
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
// função recursiva de processamento do grafo
// ao encontrar um nó, aplica as definições declaradas e itera sobre os seus descendentes
// nota: se um nó A, declarado em 'Nodes' aparecer instanciado uma vez no grafo com descendentes B e C
// e posteriormente o nó A for novamente instanciado, a relação de descendência com B e C não é implícita.
// Neste modelo o nó é só uma transformação geométrica ou de atributos, não uma definição de um sub-grafo 
// com vários nós que pode ser reinstanciado
{
	TiXmlElement *child=parent->FirstChildElement();

	// cria um prefixo de espaços para indentação, apenas para visualização
	int prefixLen=nivel*2;
	char prefix[100];
	memset(prefix,' ',prefixLen);
	prefix[prefixLen]=0;

	while (child)
	{
		if (strcmp(child->Value(),"Node")==0)
		{
			// é um nó
			printf("%s Nodo do tipo '%s' com id '%s'\n",prefix, child->Value(), child->Attribute("id"));
			// acede aos dados do nó e aplica materiais, texturas, transformações
			// para aceder ao nó, existiriam várias alternativas
			// aqui implementa-se uma pesquisa básica
			TiXmlElement *node=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

			if (node)
			{
				printf("%s     - Material id: '%s' \n", prefix, node->FirstChildElement("material")->Attribute("id"));
				printf("%s     - Texture id: '%s' \n", prefix, node->FirstChildElement("texture")->Attribute("id"));

				// repetir para outros detalhes do nó
			}


			// processa recursivamente os seus descendentes
			processGraphNode(child,nivel+1);
		}
		if (strcmp(child->Value(),"Leaf")==0)
		{
			// é folha
			printf("%s Folha do tipo '%s' com id '%s'\n",prefix, child->Value(), child->Attribute("id"));

			// acede aos dados declarados na secção Leaves
			TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

			if (leaf)
			{
				printf("%s     - tipo de folha: '%s' \n", prefix, leaf->Attribute("type"));
				// repetir para outros detalhes do nó
			}
			
			
			// e faz o render propriamente dito de acordo com o tipo de primitiva
			
		}

		child=child->NextSiblingElement();
	}

}
*/

void processGlobals(void)
{
	TiXmlElement* globalsElement=scene->sgxElement->FirstChildElement("globals");
		if (globalsElement)
		{
			cout << "Processing globals"<< endl;
			scene->root = globalsElement->Attribute("root");
			if (globalsElement->QueryIntAttribute("maxlights",&scene->maxlights)==TIXML_SUCCESS && 
				globalsElement->QueryIntAttribute("maxmaterials",&scene->maxmaterials)==TIXML_SUCCESS &&
				globalsElement->QueryIntAttribute("maxobjects",&scene->maxobjects)==TIXML_SUCCESS &&
				globalsElement->QueryIntAttribute("maxtextures",&scene->maxtextures)==TIXML_SUCCESS &&
				!scene->root.empty())
				cout << "globals atributes: " << scene->root << endl;
				//printf("  globals attributes: %i, %i, root: %s\n", maxlights, maxmaterials, &root);
			else
				printf("Error parsing globals\n");
		}
		else
			printf("globals not found\n");
}

void processView_translation(TiXmlElement* translateElement)
{
	// translate: exemplo para um nó com um parâmetro que aglutina vários floats
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
				cout << "Error parsing translation" << endl;
		}
		else
			printf("translate not found\n");	
}

void processView_rotation(TiXmlElement* rotateElement)
{
	// translate: exemplo para um nó com um parâmetro que aglutina vários floats
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
						cout << "Error parsing rotation" << endl;
				}
				else
					cout << "Error parsing rotation axis" << endl;
			}
			else
				cout << "Error parsing rotation axis" << endl;
		}
		else
			printf("rotation not found\n");	
}

void processView_scale(TiXmlElement* scaleElement)
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
				cout << "Error parsing scale" << endl;
		}
		else
			printf("scale not found\n");	
}

void processView(void)
{
	TiXmlElement* viewElement=scene->sgxElement->FirstChildElement("view");
		if (viewElement)
		{
			cout << "Processing view"<< endl;
			if (viewElement->QueryFloatAttribute("near",&scene->near)==TIXML_SUCCESS && 
				viewElement->QueryFloatAttribute("far",&scene->far)==TIXML_SUCCESS &&
				viewElement->QueryFloatAttribute("axisscale",&scene->axisscale)==TIXML_SUCCESS)
				cout << "view atributes: " << scene->axisscale << endl;
				//printf("  globals attributes: %i, %i, root: %s\n", maxlights, maxmaterials, &root);
			else
				printf("Error parsing view\n");
		}
		else
			printf("view not found\n");
		TiXmlElement* child = viewElement->FirstChildElement();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		while(child)
		{
			if(strcmp(child->Value(), "translation")==0)
				processView_translation(child);
			else if(strcmp(child->Value(), "rotation")==0)
				processView_rotation(child);
			else if(strcmp(child->Value(), "scale")==0)
				processView_scale(child);
			else
				cout << "Error parsing view: invalid child" << endl;
			child=child->NextSiblingElement();
		}

		glGetFloatv(GL_MODELVIEW_MATRIX, &scene->m[0][0]);
}

void processIllumination_ambient(TiXmlElement* ambient)
{
	float r,g,b,a;
	if(ambient->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
				ambient->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene->ambient[0] = r;
		scene->ambient[1] = g;
		scene->ambient[2] = b;
		scene->ambient[3] = a;

		cout << "ambient: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		cout << "Error parsing illumination: ambient not found or invalid" << endl;
}

void processIllumination_background(TiXmlElement* background)
{
	float r,g,b,a;
	if(background->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
				background->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene->background[0] = r;
		scene->background[1] = g;
		scene->background[2] = b;
		scene->background[3] = a;
		cout << "background: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		cout << "Error parsing illumination: background not found or invalid" << endl;
}

void processLight_ambient(TiXmlElement* ambient)
{
	float r,g,b,a;
	if(	ambient->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
		ambient->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
		ambient->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
		ambient->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene->lights.back().ambient[0] = r;
		scene->lights.back().ambient[1] = g;
		scene->lights.back().ambient[2] = b;
		scene->lights.back().ambient[3] = a;
		cout << "ambient: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		cout << "Error parsing Light: ambient not found or invalid" << endl;
}

void processLight_position(TiXmlElement* position)
{
	float x,y,z,w;
	if(	position->QueryFloatAttribute("x",&x)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("y",&y)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("z",&z)==TIXML_SUCCESS &&
		position->QueryFloatAttribute("w",&w)==TIXML_SUCCESS)
	{
		scene->lights.back().position[0] = x;
		scene->lights.back().position[1] = y;
		scene->lights.back().position[2] = z;
		scene->lights.back().position[3] = w;
		cout << "position: " << x << " " << y << " " << z << " " << w << endl;
	}
	else
		cout << "Error parsing Light: position not found or invalid" << endl;
}

void processLight_diffuse(TiXmlElement* diffuse)
{
	float r,g,b,a;
	if(	diffuse->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
		diffuse->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
		diffuse->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
		diffuse->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene->lights.back().diffuse[0] = r;
		scene->lights.back().diffuse[1] = g;
		scene->lights.back().diffuse[2] = b;
		scene->lights.back().diffuse[3] = a;
		cout << "diffuse: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		cout << "Error parsing Light: diffuse not found or invalid" << endl;
}

void processLight_specular(TiXmlElement* specular)
{
	float r,g,b,a;
	if(	specular->QueryFloatAttribute("r",&r)==TIXML_SUCCESS &&
		specular->QueryFloatAttribute("g",&g)==TIXML_SUCCESS &&
		specular->QueryFloatAttribute("b",&b)==TIXML_SUCCESS &&
		specular->QueryFloatAttribute("a",&a)==TIXML_SUCCESS)
	{
		scene->lights.back().specular[0] = r;
		scene->lights.back().specular[1] = g;
		scene->lights.back().specular[2] = b;
		scene->lights.back().specular[3] = a;
		cout << "specular: " << r << " " << g << " " << b << " " << a << endl;
	}
	else
		cout << "Error parsing Light: specular not found or invalid" << endl;
}

void processLights_light(TiXmlElement* light)
{
	const char* id = light->Attribute("id");
	const char* enabled = light->Attribute("enabled");

	if(id != NULL && enabled != NULL && (strcmp(enabled, "1") == 0 || strcmp(enabled, "0")))
	{
		string id2 = light->Attribute("id");
		vector<Light>::iterator it;
		for(it=scene->lights.begin() ; it < scene->lights.end(); it++)
		{
			if(it->id.compare(id2) == 0)
			{
				cout << "Error parsing light: id light already exists" << endl;
				return;
			}
		}
		cout << "Parsing light id = " << id << endl;


		if(strcmp(enabled, "1") == 0)
		{
			Light l(id2, true);
			scene->lights.push_back(l);
		}
		else
		{
			Light l(id2, false);
			scene->lights.push_back(l);
		}

		TiXmlElement* child = light->FirstChildElement();
		 
		while(child)
		{
			if(strcmp(child->Value(), "ambient")==0)
				processLight_ambient(child);
			else if(strcmp(child->Value(), "position")==0)
				processLight_position(child);
			else if(strcmp(child->Value(), "diffuse")==0)
				processLight_diffuse(child);
			else if(strcmp(child->Value(), "specular")==0)
				processLight_specular(child);
			else
				cout << "Error parsing light: invalid child" << endl;
			child = child->NextSiblingElement();
		}

	}
	else
		cout << "Error parsing light: id or enabled not found or invalid" << endl;
}

void processIllumination_lights(TiXmlElement* lights)
{
	TiXmlElement* child = lights->FirstChildElement();

	while(child)
	{
		if(strcmp(child->Value(), "light")==0)
		{
			processLights_light(child);
		}
		else
			cout << "Error parsing lights: invalid child";
		child = child->NextSiblingElement();
	}
}

void processIllumination(void)
{
	TiXmlElement* illuminationElement=scene->sgxElement->FirstChildElement("illumination");
	if (illuminationElement)
		{
			const char* doublesided = illuminationElement->Attribute("doublesided");
			const char* local = illuminationElement->Attribute("local");
			if(doublesided != NULL && local != NULL && (strcmp(doublesided, "1") == 0 || strcmp(doublesided, "0") == 0) && (strcmp(local, "1") == 0 || strcmp(local, "0") == 0))
			{
				if(strcmp(doublesided, "1") == 0) 
					scene->doublesided = true;
				else
					scene->doublesided = false;

				if(strcmp(local, "1") == 0) 
					scene->local = true;
				else
					scene->local = false;

				cout << "illumination atributes: " << scene->doublesided << " " << scene->local << endl; 

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
						cout << "Error parsing illumination: invalid child" << endl;
					child=child->NextSiblingElement();
				}

			}
			else
				cout << "Error parsing illumination: doublesided or local not found or invalid" << endl;
		}
		else
			cout << "illumination not found\n" << endl;
}


void processTextures_texture(TiXmlElement* texture)
{
	const char* id = texture->Attribute("id");
	const char* file = texture->Attribute("file");
	float s,t;
	if(	texture->QueryFloatAttribute("length_s",&s)==TIXML_SUCCESS &&
		texture->QueryFloatAttribute("length_t",&t)==TIXML_SUCCESS && id != NULL && file != NULL)
	{
		Texture t1(id,file, s,t);
		scene->textures.push_back(t1);
		cout << "Texture id: "<< scene->textures.back().id << " | file: " << scene->textures.back().file << " | length_s: " << scene->textures.back().length_s << " | length_t: " << scene->textures.back().length_t << endl;
	}
	else
		cout << "Error parsing texture: argument not found or invalid" << endl;
}


void processTexture(void)
{
	TiXmlElement* texturesElement=scene->sgxElement->FirstChildElement("textures");
	if (texturesElement)
		{
				TiXmlElement* child = texturesElement->FirstChildElement();
				while(child)
				{
					processTextures_texture(child);
					child=child->NextSiblingElement();
				}
		}
		else
			cout << "textures not found\n" << endl;
}

void loadScene(void)
{

	// Read string from file

	TiXmlDocument doc( "exemplo.xml" );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		exit( 1 );
	}

	scene->sgxElement = doc.FirstChildElement( "sgx" ); 
	scene->texturesElement = doc.FirstChildElement("textures");
	scene->materialsElement = doc.FirstChildElement("materials");
	scene->lightsElement = doc.FirstChildElement("lights");
	
	scene->objectsElement = doc.FirstChildElement("objects");

	// Inicialização
	// Um exemplo de um conjunto de nós bem conhecidos e obrigatórios

	if (scene->sgxElement == NULL)
		printf("Bloco sgx não encontrado\n");
	else
	{
		printf("Processing sgx:\n");
		// frustum: exemplo para nó com parâmetros individuais
		processGlobals();
		processView();
		processIllumination();
		processTexture();
		

	// Validação dos outros grupos seria feita aqui


	// render graph
	// iteração recursiva
	//processGraphNode(graphElement,0);
	}
}
