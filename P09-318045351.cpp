/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

float movLetrero, giraLetrero, movCamion, rotllantaCamion, giraCamion;
float movCamion2, movCamion3, movCamion4;
float movLetreroOffset, giraLetreroOffset, movCamionOffset, rotllantaCamionOffset, giraCamionOffset;
bool baja, gira, avanzaCamion1, avanzaCamion2, avanzaCamion3, avanzaCamion4, vuelta;

float caeDado, giraDado;
float caeDadoOffset, giraDadoOffset;
bool caer, girar;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture Blanco;
Texture Casilla1;
Texture Casilla2;
Texture Casilla3;
Texture dadoDiezTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Arco_M;
Model Letrero_M;
Model Camion_M;
Model LlantaCamion_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void Casillas() {
	unsigned int Casillas1_indices[] = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,

		0,1,5, 0,4,5,
		1,2,6, 1,5,6,
		0,3,4, 3,4,7,
		2,3,6, 3,6,7,
	};
	GLfloat Casillas1_vertices[] = {
		1.0f, 0.2f, 1.0f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,//A - 0
		1.0f, 0.2f,-1.0f, 1.0f,0.0f, 0.0f,-1.0f,0.0f,//B - 1
	   -1.0f, 0.2f,-1.0f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,//C - 2
	   -1.0f, 0.2f, 1.0f, 0.0f,1.0f, 0.0f,-1.0f,0.0f,//D - 3

		1.0f, 0.0f, 1.0f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,//E - 4
		1.0f, 0.0f,-1.0f, 1.0f,0.0f, 0.0f,-1.0f,0.0f,//F - 5
	   -1.0f, 0.0f,-1.0f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,//G - 6
	   -1.0f, 0.0f, 1.0f, 0.0f,1.0f, 0.0f,-1.0f,0.0f,//H - 7

	};
	Mesh* Casillas1 = new Mesh();
	Casillas1->CreateMesh(Casillas1_vertices, Casillas1_indices, 64, 36);
	meshList.push_back(Casillas1);

	unsigned int Casillas2_indices[] = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,

		0,1,5, 0,4,5,
		1,2,6, 1,5,6,
		0,3,4, 3,4,7,
		2,3,6, 3,6,7,
	};
	GLfloat Casillas2_vertices[] = {
		1.0f, 0.2f, 0.5f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,//A - 0
		1.0f, 0.2f,-0.5f, 1.0f,0.0f, 0.0f,-1.0f,0.0f,//B - 1
	   -1.0f, 0.2f,-0.5f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,//C - 2
	   -1.0f, 0.2f, 0.5f, 0.0f,1.0f, 0.0f,-1.0f,0.0f,//D - 3

		1.0f, 0.0f, 0.5f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,//E - 4
		1.0f, 0.0f,-0.5f, 1.0f,0.0f, 0.0f,-1.0f,0.0f,//F - 5
	   -1.0f, 0.0f,-0.5f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,//G - 6
	   -1.0f, 0.0f, 0.5f, 0.0f,1.0f, 0.0f,-1.0f,0.0f,//H - 7
	};
	Mesh* Casillas2 = new Mesh();
	Casillas2->CreateMesh(Casillas2_vertices, Casillas2_indices, 144, 90);
	meshList.push_back(Casillas2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado10Lados()
{
	unsigned int cubo10_indices[] = {
		// Cara 1 - Numero 4
		0, 1, 2,

		// Cara 2 - Numero 6
		3, 4, 5,

		// Cara 3 - Numero 2
		6, 7, 8,

		// Cara 4 - Numero 8
		9, 10, 11,

		// Cara 5 - Numero 10
		12, 13, 14,

		// Cara 6 - Numero 5
		15, 16, 17,

		// Cara 7 - Numero 7
		18, 19, 20,

		// Cara 8 - Numero 1
		21, 22, 23,

		// Cara 9 - Numero 3
		24, 25, 26,

		// Cara 10 - Numero 9
		27, 28, 29
	};

	GLfloat cubo10_vertices[] = {
		// Cara 1 - Numero 4
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-4.55f,		0.0f,	 4.81f,		0.01f,	0.51f,		1.0f,	-1.0f,	-1.0f,  //0
		 1.63f,		0.0f,	 4.66f,		0.19f,	0.51f,		1.0f,	-1.0f,	-1.0f,  //1
		-1.56f,		5.0f,	 0.48f,		0.10f,	0.74f,		1.0f,	-1.0f,	-1.0f,  //2

		// Cara 2 - Numero 6
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		 1.63f,		0.0f,	 4.66f,		0.21f,	0.51f,		-1.0f,	-1.0f,	-1.0f,  //3
		 3.41f,		0.0f,	-1.26f,		0.39f,	0.51f,		-1.0f,	-1.0f,	-1.0f,  //4
		-1.56f,		5.0f,	 0.48f,		0.30f,	0.74f,		-1.0f,	-1.0f,	-1.0f,  //5

		// Cara 3 - Numero 2
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		 3.41f,		0.0f,	-1.26f,		0.41f,	0.51f,		-1.0f,	-1.0f,	1.0f,  //6
		-1.68f,		0.0f,	-4.78f,		0.59f,	0.51f,		-1.0f,	-1.0f,	1.0f,  //7
		-1.56f,		5.0f,	 0.48f,		0.50f,	0.74f,		-1.0f,	-1.0f,	1.0f,  //8

		// Cara 4 - Numero 8
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-1.68f,		0.0f,	-4.78f,		0.61f,	0.51f,		1.0f,	-1.0f,	1.0f,  //9
		-6.60f,		0.0f,	-1.03f,		0.79f,	0.51f,		1.0f,	-1.0f,	1.0f,  //10
		-1.56f,		5.0f,	 0.48f,		0.70f,	0.74f,		1.0f,	-1.0f,	1.0f,  //11

		// Cara 5 - Numero 10
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-6.60f,		0.0f,	-1.03f,		0.81f,	0.51f,		1.0f,	-1.0f,	-1.0f,  //12
		-4.55f,		0.0f,	 4.81f,		0.99f,	0.51f,		1.0f,	-1.0f,	-1.0f,  //13
		-1.56f,		5.0f,	 0.48f,		0.90f,	0.74f,		1.0f,	-1.0f,	-1.0f,  //14

		// Cara 6 - Numero 5
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-4.55f,		0.0f,	 4.81f,		0.01f,	0.49f,		1.0f,	1.0f,	-1.0f,  //15
		 1.63f,		0.0f,	 4.66f,		0.19f,	0.49f,		1.0f,	1.0f,	-1.0f,  //16
		-1.56f,	   -5.0f,	 0.48f,		0.10f,	0.26f,		1.0f,	1.0f,	-1.0f,  //17

		// Cara 7 - Numero 7
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		 1.63f,		0.0f,	 4.66f,		0.21f,	0.49f,		-1.0f,	1.0f,	-1.0f,  //18
		 3.41f,		0.0f,	-1.26f,		0.39f,	0.49f,		-1.0f,	1.0f,	-1.0f,  //19
		-1.56f,	   -5.0f,	 0.48f,		0.30f,	0.26f,		-1.0f,	1.0f,	-1.0f,  //20

		// Cara 8 - Numero 1
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		 3.41f,		0.0f,	-1.26f,		0.41f,	0.49f,		-1.0f,	1.0f,	1.0f,  //21
		-1.68f,		0.0f,	-4.78f,		0.59f,	0.49f,		-1.0f,	1.0f,	1.0f,  //22
		-1.56f,	   -5.0f,	 0.48f,		0.50f,	0.26f,		-1.0f,	1.0f,	1.0f,  //23

		// Cara 9 - Numero 3
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-1.68f,		0.0f,	-4.78f,		0.61f,	0.49f,		1.0f,	1.0f,	1.0f,  //24
		-6.60f,		0.0f,	-1.03f,		0.79f,	0.49f,		1.0f,	1.0f,	1.0f,  //25
		-1.56f,	   -5.0f,	 0.48f,		0.70f,	0.26f,		1.0f,	 1.0f,	1.0f,  //26

		// Cara 10 - Numero 9
		// X		  Y		    Z		  S		  T			 NX		 NY		 NZ
		-6.60f,		0.0f,	-1.03f,		0.81f,	0.49f,		1.0f,	1.0f,	-1.0f,  //27
		-4.55f,		0.0f,	 4.81f,		0.99f,	0.49f,		1.0f,	1.0f,	-1.0f,  //28
		-1.56f,	   -5.0f,	 0.48f,		0.90f,	0.26f,		1.0f,	1.0f,	-1.0f   //29

	};

	Mesh* dado10lados = new Mesh();
	dado10lados->CreateMesh(cubo10_vertices, cubo10_indices, 300, 30);
	meshList.push_back(dado10lados);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	Casillas();
	CrearDado10Lados();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	dadoDiezTexture = Texture("Textures/dado10lados.tga");
	dadoDiezTexture.LoadTextureA();

	Blanco = Texture("Textures/Blanco.tga");
	Blanco.LoadTextureA();
	Casilla1 = Texture("Textures/Casilla1.tga");
	Casilla1.LoadTextureA();
	Casilla2 = Texture("Textures/Casilla2.tga");
	Casilla2.LoadTextureA();
	Casilla3 = Texture("Textures/Casilla3.tga");
	Casilla3.LoadTextureA();

	

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Arco_M = Model();
	Arco_M.LoadModel("Models/Arco_obj.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero_obj.obj");
	Camion_M = Model();
	Camion_M.LoadModel("Models/Camion_obj.obj");
	LlantaCamion_M = Model();
	LlantaCamion_M.LoadModel("Models/LlantaCamion_obj.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movCoche = 0.0f;
	movOffset = 0.3f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true;

	movLetrero = 0.0f;
	movLetreroOffset = 0.06f;
	giraLetrero = 370.0f;
	giraLetreroOffset = 2.6f;
	baja = true;
	gira = true;

	movCamion = 0.0f;
	movCamionOffset = 0.05f;
	rotllantaCamion = 0.0f;
	rotllantaCamionOffset = 1.5f;
	giraCamion = 0.0;
	giraCamionOffset = 0.37f;
	avanzaCamion1 = true;
	vuelta = false;

	caeDado = 0.0f;
	caeDadoOffset = 0.05f; 
	giraDado = 0.0f;
	giraDadoOffset = 0.5f;
	caer = true; 
	girar = true;

	glfwSetTime(0);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
	//Movimineto Coche Profesor
#if 0 //Mover Carro Ejemplo
		if (glfwGetTime() > 5)
		{
			if (avanza)
			{
				if (movCoche > -305.0f)
				{
					movCoche -= movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta += rotllantaOffset * deltaTime;
				}
				else
				{
					avanza = !avanza;
				}
			}
			else
			{
				if (movCoche < 280.0f)
				{
					movCoche += movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta -= rotllantaOffset * deltaTime;
				}
				else
				{
					avanza = !avanza;
				}
			}
		}
#endif

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxLetrero(1.0);
		glm::mat4 modelauxCamion(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// Coche Profesor
#if 0
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
#endif

		// - - - - - - - - TABLERO - - - - - - - - //
#if 1 // Mapa de Monopoly
		//-----

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla1.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla3.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//-------

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla3.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//-------

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla3.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//-------

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-1.5f, -0.34f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Casilla2.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh(); 
#endif
		// - - - - - - - - ARCO - - - - - - - - //
		
		giraLetrero += giraLetreroOffset * deltaTime;
		//if (glfwGetTime() > 2 )
		{
			if (baja)
			{
				if (movLetrero > 0.0f)
				{
					movLetrero -= movLetreroOffset * deltaTime;
					//printf("movLetrero%f \n ", movLetrero);
					giraLetrero = 0;
				}
				else
				{
					baja = !baja;
					//glfwSetTime(0);
					giraLetrero = 0;
				}
			}
			else 
			{
				if (movLetrero < 9.0f)
				{
					giraLetrero = 0;
					movLetrero += movLetreroOffset * deltaTime;
					//printf("movLetrero%f \n ", movLetrero);
					
				}
				else
				{
					baja = !baja;
					//glfwSetTime(0);
					giraLetrero = 0;
				}
			}
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelauxLetrero = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();
		
		// - - - - - - - - LETRERO - - - - - - - - //
		model = modelauxLetrero;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f - movLetrero, 0.8));
		model = glm::rotate(model, giraLetrero * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		// - - - - - - - - CAMIÓN 6 RUEDAS - - - - - - - - //
			
		float contador = glfwGetTime(), lugarCamion = 0.0f;

		printf("%f\n", glfwGetTime());
		rotllantaCamion += rotllantaCamionOffset * deltaTime;
		if (avanzaCamion1)
		{
			if (glfwGetTime() < 15.0) {
				movCamion += movCamionOffset * deltaTime;
			}
			else {
				avanzaCamion1 = !avanzaCamion1;
				glfwSetTime(0);
			}
		}
		else {

			if (glfwGetTime() < 2) {
				giraCamion += giraCamionOffset * deltaTime ;
			}
			else {
				avanzaCamion1 = !avanzaCamion1;
				glfwSetTime(0);
			}
		}
		

		//Carroceria
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		model = glm::rotate(model, giraCamion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f, -1.0f, 5.0 - movCamion));
		
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		
		modelauxCamion = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camion_M.RenderModel();

		// Llantas //
		//Izquierdas
		model = modelauxCamion;
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, -23.0));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		model = modelauxCamion;
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, 8.45));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		model = modelauxCamion;
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, 19.6));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		//Derechas
		model = modelauxCamion;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, 23.0));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		model = modelauxCamion;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, -8.45));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		model = modelauxCamion;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-9.0f, 5.0f, -19.6));
		model = glm::rotate(model, rotllantaCamion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaCamion_M.RenderModel();

		// - - - - - - - - DADO DE 10 LADOS - - - - - - - - //
		if (glfwGetTime() > 4 and glfwGetTime() < 5.5 ) {
			if (caer) {
				giraDado = 0.0f;
				caeDado += caeDadoOffset * deltaTime;
			}
			else {
				caer = !caer;
			}
		}
		else if (glfwGetTime() > 5.5 and glfwGetTime() < 10) {
			if (girar) {
				giraDado += giraDadoOffset * deltaTime;
			}
			else {
				girar = !girar;
			}
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 5.0f - caeDado, 15.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, giraDado * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoDiezTexture.UseTexture();
		meshList[6]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
