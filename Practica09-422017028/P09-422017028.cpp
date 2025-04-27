/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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
#include<assimp/Importer.hpp>

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
float movDragon;		//Control del movimiento para que avance y regrese
bool avanzaDragon;		//Bandera para avanzar o retroceder Dragon
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float wingAngle = 0.0f;
bool wingUp = true;
//Variables para animacion espiral de la cabeza roja
float tCabezaRoja = 0.0f;  // Tiempo acumulado para la cabeza roja
float radioBaseRoja = 0.0f;  // Radio base de la espiral
float incrementoRadioRoja = 0.0f;  // Cuánto se incrementa el radio por vuelta
float velocidadAngularRoja = 0.0f;  // Velocidad de rotación de la espiral
//Variables para animacíon senoidal de la cabeza azul
float tCabezaAzul = 0.0f;
float velocidadAngularAzul = 0.0f;  // Más lento que la roja
//Variables para animacion eliptica de la cabeza verde
float tCabezaVerde = 0.0f;
float velocidadAngularVerde = 0.0f;
//Variables para animacion lemnistica de la cabeza blanca
float tCabezaBlanca = 0.0f;
float velocidadAngularBlanca = 0.0f;
//Variables para animacion con movimiento aleatorio de la cabeza cafe
float tCabezaCafe = 0.0f;
float velocidadAngularCafe = 0.0f;
float objetivoRotacionCafe = 0.0f;
float rotacionActualCafe = 0.0f;
//Variables para la animación del letrero
float toffsetLetrau = 0.0f;
float toffsetLetrav = 0.0f;
//Arreglos para coordenadas U,V que nos guardan la posición de las letras a imprimir
//PROYECTO CGEIHC FERIA---> 19 caracteres (sin contar espacios)
float incrementoTipografia_U[] = {
	0.281f, 0.567f, 0.142f, 0.68f, 0.60f, 0.315f, 0.858f, 0.142f,		//PROYECTO
	0.315f, 0.85f, 0.60f, 0.143f, 0.04f, 0.315f,						//CGEIHC
	0.7f, 0.572f, 0.567f, 0.143f, 0.02f									//FERIA
};

float incrementoTipografia_V[] = {
	0.5f, 0.5f, 0.5f, 0.75f, 0.0f, 0.0f, 0.5f, 0.5f,					//PROYECTO
	0.0f, 0.0f, 0.0f, 0.25f, 0.25f, 0.0f,								//CGEIHC
	0.0f, 0.0f, 0.5f, 0.25f, 0.0f										//FERIA
};

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture LetrasTexture;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Model AlaIzq_M;
Model AlaDer_M;
Model Puerta_M;
Model Letrero_M;
Model Rojo_M;
Model Azul_M;
Model Verde_M;
Model Blanco_M;
Model Cafe_M;

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

//cálculo del promedio de las normales para sombreado de Phong
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

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int letraIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat letraVertices[] = {
	-0.5f, 0.0f, 0.5f,    0.0f, 0.75f,		0.0f, -1.0f, 0.0f,
	 0.5f, 0.0f, 0.5f,    0.143f, 0.75f,    0.0f, -1.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,   0.143f,1.0f,		0.0f, -1.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,   0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(letraVertices, letraIndices, 32, 6);
	meshList.push_back(obj8);  // será meshList[7]
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	LetrasTexture = Texture("Textures/letras.png");
	LetrasTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/CuerpoDragon.obj");
	AlaIzq_M = Model();
	AlaIzq_M.LoadModel("Models/AlaIzquierda.obj");
	AlaDer_M = Model();
	AlaDer_M.LoadModel("Models/AlaDerecha.obj");
	Puerta_M = Model();
	Puerta_M.LoadModel("Models/puertarejas.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/letrero.obj");
	Rojo_M = Model();
	Rojo_M.LoadModel("Models/CabezaRoja.obj");
	Azul_M = Model();
	Azul_M.LoadModel("Models/CabezaAzul.obj");
	Verde_M = Model();
	Verde_M.LoadModel("Models/CabezaVerde.obj");
	Blanco_M = Model();
	Blanco_M.LoadModel("Models/CabezaBlanca.obj");
	Cafe_M = Model();
	Cafe_M.LoadModel("Models/CabezaCafe.obj");

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
		0.0f, 2.5f, 1.5f,
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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.05f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	//Variables para el movimiento del dragon
	movDragon = 0.0f;
	avanzaDragon = true;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		//Cabeza roja
		// Parámetros locales ajustados directamente (más lentos)
		velocidadAngularRoja = 0.1f;
		radioBaseRoja = 0.05f;
		incrementoRadioRoja = 0.005f;
		// Actualizar tiempo para la espiral de Arquímedes
		tCabezaRoja += velocidadAngularRoja * deltaTime;
		// Calcular ángulo y radio para la espiral
		float anguloRoja = tCabezaRoja;
		float radioRoja = radioBaseRoja + incrementoRadioRoja * tCabezaRoja;
		// Movimiento lento de la cabeza roja: rotación sobre eje Y
		float rotacionCabezaRoja = sin(anguloRoja) * 10.0f;  // Suave

		//Cabeza azul
		// Parámetro local ajustado directamente (más lento)
		velocidadAngularAzul = 0.2f;  // Controla qué tan rápido se balancea
		// Animación senoidal para la cabeza azul
		tCabezaAzul += velocidadAngularAzul * deltaTime;
		float rotacionCabezaAzul = sin(tCabezaAzul) * 15.0f;  // Movimiento lateral tipo péndulo

		//Cabeza verde
		// Parámetro local ajustado directamente (más lento que la azul)
		velocidadAngularVerde = 0.15f;
		// Animación elíptica para la cabeza verde (rota en X y Z simultáneamente)
		tCabezaVerde += velocidadAngularVerde * deltaTime;
		float rotacionCabezaVerdeX = sin(tCabezaVerde) * 8.0f;  // Oscilación en X
		float rotacionCabezaVerdeZ = cos(tCabezaVerde) * 8.0f;  // Oscilación en Z

		//Cabeza blanca
		// Parámetro local ajustado directamente (más lento)
		velocidadAngularBlanca = 0.05f;
		// Animación tipo lemniscata para la cabeza blanca ( figura de 8)
		tCabezaBlanca += velocidadAngularBlanca * deltaTime;
		// Movimiento en lemniscata: (x = a * sin(t)) (z = a * sin(t) * cos(t))
		float a = 0.1f;  // Escala de la figura
		float rotacionCabezaBlancaY = sin(tCabezaBlanca) * 10.0f;  // Rotación suave en Y

		//Cabeza cafe
		// Parámetros locales ajustados directamente
		velocidadAngularCafe = 0.1f;
		tCabezaCafe += deltaTime;
		// Cada cierto tiempo, cambiar el objetivo aleatorio
		if (fmod(tCabezaCafe, 5.0f) < deltaTime) {  // Cada 5 segundos aprox.
			objetivoRotacionCafe = ((rand() % 20) - 10);  // Entre -10 y 10 grados
		}
		// Suavizar la rotación actual hacia el objetivo
		rotacionActualCafe += (objetivoRotacionCafe - rotacionActualCafe) * 0.02f;  // Suavizado lento

		//Aleteo
		if (wingUp) {
			wingAngle += 5.0f * deltaTime;
			if (wingAngle >= 30.0f) wingUp = false;
		}
		else {
			wingAngle -= 5.0f * deltaTime;
			if (wingAngle <= -30.0f) wingUp = true;
		}

		if (avanzaDragon) {
			if (movDragon > -100.0f) {
				movDragon -= movOffset * deltaTime;
			}
			else {
				avanzaDragon = false;
			}
		}
		else if (movDragon < 100.0f) {
			movDragon += movOffset * deltaTime;
		}
		else {
			avanza = true;
		}

		//Coche
		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche - 50.0f, 0.5f, -2.0f));
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

		//Agregar cuerpo de dragon, cabezas y sus alas de manera jerarquica
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movDragon + 0.0f, 5.0f + sin(glm::radians(angulovaria)), 6.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.38f, 1.8f, 0.1f));
		model = glm::rotate(model, glm::radians(wingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaIzq_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, 1.6f, -0.1f));
		model = glm::rotate(model, glm::radians(-wingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, 1.32f, 0.19f));  // Posición base
		model = glm::rotate(model, glm::radians(rotacionCabezaRoja), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rojo_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.32f, 1.51f, -0.13f));  // Posición base
		model = glm::rotate(model, glm::radians(rotacionCabezaVerdeX), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación en X
		model = glm::rotate(model, glm::radians(rotacionCabezaVerdeZ), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotación en Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Verde_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.23f, 1.31f, -0.49f));  // Posición base
		model = glm::rotate(model, glm::radians(rotacionCabezaAzul), glm::vec3(0.0f, 0.0f, 1.0f));  // Balanceo sobre Z
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Azul_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.08f, 1.9f, -0.42f));  // Posición base
		model = glm::rotate(model, glm::radians(rotacionActualCafe), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación aleatoria lateral
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cafe_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.05f, 1.88f, 0.18f));  // Posición FIJA
		model = glm::rotate(model, glm::radians(rotacionCabezaBlancaY), glm::vec3(0.0f, 1.0f, 0.0f));  // Solo gira, sin moverse
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blanco_M.RenderModel();

		//Agregar puerta letrero de manera jerarquica
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, -1.9f, 2.0f));  
		modelaux = model;
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.8f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));     
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		static float tiempoNumero = 0.0f;
		static float intervaloNumero = 15.0f;

		tiempoNumero += deltaTime;
		if (tiempoNumero >= intervaloNumero) {
			toffsetnumerocambiau += 0.25f;
			if (toffsetnumerocambiau > 1.0f)
				toffsetnumerocambiau = 0.0f;

			tiempoNumero = 0.0f;
		}

		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		static float tiempoCambioTextura = 0.0f;
		static float intervaloCambio = 20.0f;
		static bool mostrarNumero1 = true;

		tiempoCambioTextura += deltaTime;
		if (tiempoCambioTextura >= intervaloCambio) {
			mostrarNumero1 = !mostrarNumero1; // Alternar valor
			tiempoCambioTextura = 0.0f;
		}

		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		if (mostrarNumero1) {
			Numero1Texture.UseTexture();
		}
		else {
			Numero2Texture.UseTexture();
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Dibujo de las letras animadas tipo letrero LED/LCD
		static int palabraActual = 0;  // 0 = PROYECTO, 1 = CGEIHC, 2 = FERIA
		static float tiempoLetras = 0.0f;
		float intervaloLetras = 10.0f;  // Tiempo entre aparición de letras

		static int letrasMostradas = 1;  // Letras visibles por palabra
		static float desplazamientoLetras = 0.0f;
		float velocidadDesplazamiento = 0.00001f * deltaTime;
		desplazamientoLetras -= velocidadDesplazamiento;

		// Cambio de letra por tiempo
		tiempoLetras += deltaTime;
		if (tiempoLetras >= intervaloLetras) {
			letrasMostradas++;
			int totalLetrasPalabra = (palabraActual == 0) ? 8 : (palabraActual == 1) ? 6 : 5;

			if (letrasMostradas > totalLetrasPalabra) {
				letrasMostradas = 1;  // Reinicia letras visibles para la siguiente palabra
				palabraActual = (palabraActual + 1) % 3;  // Cambia de palabra
				desplazamientoLetras = 0.0f;  // Reinicia desplazamiento
			}

			tiempoLetras = 0.0f;
		}

		// Configuración de posición y escala
		float centroLetreroX = 8.0f;
		float alturaBaseLetrero = 11.5f;
		float profundidadLetrero = 3.0f;
		float espacioEntreLetras = 0.8f;
		float escalaLetra = 1.0f;

		toffsetLetrau = 0.0f;
		toffsetLetrav = 0.0f;

		int inicioLetra = 0;
		float alturaLetrero = alturaBaseLetrero;
		int totalLetras = 0;

		if (palabraActual == 0) {  // PROYECTO
			inicioLetra = 1;
			alturaLetrero = alturaBaseLetrero;
			totalLetras = 8;
		}
		else if (palabraActual == 1) {  // CGEIHC
			inicioLetra = 9;
			alturaLetrero = alturaBaseLetrero - 1.1f;
			totalLetras = 6;
		}
		else {  // FERIA
			inicioLetra = 15;
			alturaLetrero = alturaBaseLetrero - 2.1f;
			totalLetras = 5;
		}

		int primeraLetra = inicioLetra;
		int ultimaLetra = inicioLetra + letrasMostradas - 1;

		// Centrado horizontal
		float anchoLinea = (totalLetras - 1) * espacioEntreLetras;
		float desplazamientoInicialLinea = centroLetreroX - (anchoLinea / 2.0f);

		//  Recorrer de derecha a izquierda dentro de cada palabra
		for (int k = ultimaLetra; k >= primeraLetra; k--) {
			int posicionEnLinea = k - inicioLetra;
			float xOffset = desplazamientoInicialLinea + (posicionEnLinea * espacioEntreLetras) + desplazamientoLetras;

			toffsetLetrau += incrementoTipografia_U[k - 1];
			toffsetLetrav -= incrementoTipografia_V[k - 1];

			toffset = glm::vec2(toffsetLetrau, toffsetLetrav);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(xOffset, alturaLetrero, profundidadLetrero));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escalaLetra, escalaLetra, escalaLetra));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			LetrasTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[7]->RenderMesh();

			// Reinicia UV
			toffsetLetrau -= incrementoTipografia_U[k - 1];
			toffsetLetrav += incrementoTipografia_V[k - 1];
		}

		glDisable(GL_BLEND);

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}