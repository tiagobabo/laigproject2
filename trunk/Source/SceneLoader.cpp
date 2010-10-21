#include "SceneLoader.h"

// Apontadores para os principais grupos
// declarados aqui como globais por conveni�ncia
// idealmente tudo seria inclu�do numa classe

//-------------------------------------------------------

Scene* scene = new Scene();

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
	// translate: exemplo para um n� com um par�metro que aglutina v�rios floats
		if (translateElement)
		{
			if(translateElement->QueryFloatAttribute("x",&scene->valX)==TIXML_SUCCESS &&
				translateElement->QueryFloatAttribute("y",&scene->valY)==TIXML_SUCCESS &&
				translateElement->QueryFloatAttribute("z",&scene->valZ)==TIXML_SUCCESS)
					cout << "ValX: " << scene->valX  << " |ValY: " << scene->valY << " |ValZ: " << scene->valZ << endl;
			else
				cout << "Error parsing translation" << endl;
		}
		else
			printf("translate not found\n");	
}

void processView_rotation(TiXmlElement* rotateElement)
{
	// translate: exemplo para um n� com um par�metro que aglutina v�rios floats
		if (rotateElement)
		{
			scene->axis = rotateElement->Attribute("axis");
			if(rotateElement->QueryFloatAttribute("angle",&scene->angle)==TIXML_SUCCESS &&
				scene->axis!=NULL)
					cout << "RotAxis: " << scene->axis  << " |Ang: " << scene->angle << endl;
			else
				cout << "Error parsing rotation" << endl;
		}
		else
			printf("rotation not found\n");	
}

void processView_scale(TiXmlElement* scaleElement)
{
	if (scaleElement)
		{
			if(scaleElement->QueryFloatAttribute("x",&scene->scaleX)==TIXML_SUCCESS &&
				scaleElement->QueryFloatAttribute("y",&scene->scaleY)==TIXML_SUCCESS &&
				scaleElement->QueryFloatAttribute("z",&scene->scaleZ)==TIXML_SUCCESS)
					cout << "ScaleX: " << scene->scaleX  << " |ScaleY: " << scene->scaleY << " |ScaleZ: " << scene->scaleZ << endl;
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

		while(child)
		{
			if(strcmp(child->Value(), "translation")==0)
			{
				processView_translation(child);
			}
			else if(strcmp(child->Value(), "rotation")==0)
			{
				processView_rotation(child);
			}
			else if(strcmp(child->Value(), "scale")==0)
			{
				processView_scale(child);
			}
			child=child->NextSiblingElement();
		}

			

		// repetir para cada uma das vari�veis
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

	// Inicializa��o
	// Um exemplo de um conjunto de n�s bem conhecidos e obrigat�rios

	if (scene->sgxElement == NULL)
		printf("Bloco sgx n�o encontrado\n");
	else
	{
		printf("Processing sgx:\n");
		// frustum: exemplo para n� com par�metros individuais
		processGlobals();
		processView();

		

	// Valida��o dos outros grupos seria feita aqui


	// render graph
	// itera��o recursiva
	//processGraphNode(graphElement,0);
	}
}
