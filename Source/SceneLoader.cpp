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
	// translate: exemplo para um nó com um parâmetro que aglutina vários floats
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

			

		// repetir para cada uma das variáveis
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

		

	// Validação dos outros grupos seria feita aqui


	// render graph
	// iteração recursiva
	//processGraphNode(graphElement,0);
	}
}
