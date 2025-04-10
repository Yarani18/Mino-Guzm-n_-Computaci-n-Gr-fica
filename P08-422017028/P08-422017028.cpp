/*
Práctica 8: Iluminación 2 
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaedroTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Auto_M;
Model LlantaDD_M;
Model LlantaDI_M;
Model LlantaTD_M;
Model LlantaTI_M;
Model Cofre_M;
Model Lampara_M;
Model Octaedro_M;
Model Fuego_M;

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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, -1.0f,


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

void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		// Parte superior
		0, 1, 2,     // Cara 1
		3, 4, 5,     // Cara 2
		6, 7, 8,     // Cara 3
		9,10,11,     // Cara 4
		12,13,14,    // Cara 5
		15,16,17,    // Cara 6
		18,19,20,    // Cara 7
		21,22,23     // Cara 8

	};

	GLfloat octaedro_vertices[] = {
		// Cara 1: top - left - front   normal: (1, -1, -1)
		0.0f,  1.0f,  0.0f,  0.14f,	0.65f,   1.0f,  -1.0f,  -1.0f,
		-1.0f, 0.0f,  0.0f,  0.02f,	0.35f,   1.0f,  -1.0f,  -1.0f,
		0.0f,  0.0f,  1.0f,  0.26f,	0.35f,   1.0f,  -1.0f,  -1.0f,
		
		// Cara 2: top - front - right   normal: (-1, -1, -1)
		0.0f,  1.0f,  0.0f,  0.28f, 0.35f,   -1.0f,  -1.0f,  -1.0f,
		0.0f,  0.0f,  1.0f,  0.40f, 0.65f,   -1.0f,  -1.0f,  -1.0f,
		1.0f,  0.0f,  0.0f,  0.16f, 0.65f,   -1.0f,  -1.0f,  -1.0f,

		// Cara 3: top - right - back   normal: (-1, -1, 1)
		0.0f,  1.0f,  0.0f,  0.43f, 0.65f,   -1.0f,  -1.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  0.3f,  0.35f,   -1.0f,  -1.0f,  1.0f,
		0.0f,  0.0f, -1.0f,  0.55f, 0.35f,   -1.0f,  -1.0f,  1.0f,

		// Cara 4: top - back - left   normal: (1, -1, 1)
		0.0f,  1.0f,  0.0f,  0.57f, 0.35f,   1.0f,  -1.0f,  1.0f,
		0.0f,  0.0f, -1.0f,  0.65f, 0.65f,   1.0f,  -1.0f,  1.0f,
		-1.0f, 0.0f,  0.0f,	0.45f, 0.65f,    1.0f,  -1.0f,  1.0f,

		// Cara 5: bottom - front - left   normal: (1, 1, -1)
		0.0f, -1.0f,  0.0f,  0.71f, 0.65f,   1.0f,  1.0f,  -1.0f,
		0.0f,  0.0f,  1.0f,  0.61f, 0.35f,   1.0f,  1.0f,  -1.0f,
		-1.0f, 0.0f,  0.0f,  0.81f, 0.35f,   1.0f,  1.0f,  -1.0f,

		// Cara 6: bottom - right - front   normal: (-1, 1, -1)
		0.0f, -1.0f,  0.0f,  0.86f, 0.35f,   -1.0f,  1.0f,  -1.0f,
		1.0f,  0.0f,  0.0f,  0.95f, 0.65f,   -1.0f,  1.0f,  -1.0f,
		0.0f,  0.0f,  1.0f,  0.79f, 0.65f,   -1.0f,  1.0f,  -1.0f,

		// Cara 7: bottom - back - right   normal: (-1, 1, 1)
		0.0f, -1.0f,  0.0f,  0.57f, 0.95f,   -1.0f,  1.0f,  1.0f,
		0.0f,  0.0f, -1.0f,  0.67f, 0.69f,   -1.0f,  1.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  0.45f, 0.69f,   -1.0f,  1.0f,  1.0f,

		// Cara 8: bottom - left - back   normal: (-1, 1, -1)
		0.0f, -1.0f,  0.0f,  0.43f, 0.01f,   1.0f,  1.0f,  1.0f,
		-1.0f, 0.0f,  0.0f,  0.3f,  0.30f,   1.0f,  1.0f,  1.0f,
		0.0f,  0.0f, -1.0f,  0.55f, 0.30f,   1.0f,  1.0f,  1.0f
		
	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);
};

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();

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
	octaedroTexture = Texture("Textures/octaedro_numeros.png");
	octaedroTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Auto_M = Model();
	Auto_M.LoadModel("Models/auto.obj");
	LlantaDD_M = Model();
	LlantaDD_M.LoadModel("Models/LlantaDDtext.obj");
	LlantaDI_M = Model();
	LlantaDI_M.LoadModel("Models/LlantaDItext.obj");
	LlantaTD_M = Model();
	LlantaTD_M.LoadModel("Models/LlantaTDtext.obj");
	LlantaTI_M = Model();
	LlantaTI_M.LoadModel("Models/LlantaTItext.obj");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/Cofrecars.obj");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lamparalila.obj");
	Fuego_M = Model();
	Fuego_M.LoadModel("Models/rojotext.obj");

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

	//Luz puntual blanca para la lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, // Color blanco
		1.0f, 1.0f, // Intensidad difusa y especular
		10.5f, 5.0f, 3.0f, // Posición en la parte superior de la lámpara
		0.3f, 0.2f, 0.1f); // Atenuación (constante, lineal, cuadrática)
	pointLightCount++;

	//Luz puntual naranja para el fuego
	pointLights[2] = PointLight(1.0, 0.5f, 0.0f,
		1.5f, 1.5f,
		-4.5f, 1.5f, 5.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//Luz del cofre del coche
	spotLights[1] = SpotLight(1.0f, 0.5f, 0.0f, //color naranja
		1.5f, 1.5f, //intensidad difusa y especular
		7.0f, 2.0f, 2.5f, //posicion aproximada del cofre
		0.0f, -0.2f, 1.0f, //direccion hacia el frente
		1.0f, 0.1f, 0.01f, //atenuacion 
		15.0f); //angulo del cono
	spotLightCount++;
	
	//Faro del coche delantero
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f, //color azul
		1.0f, 2.0f, //intensidad difusa y especular
		7.5f, 0.0f, 0.0f, //Posicion relativa al auto
		0.0f, 0.0f, 1.2f, //Direccion hacia adelante
		1.0f, 0.0f, 0.0f, //Atenuacion
		20.0f //Angulo de apertura
	);
	spotLightCount++;

	//Faro del coche trasero
	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f, // color rojo
		1.0f, 2.0f,                            // intensidad difusa y especular
		7.5f, 0.0f , -3.0f,                     // posición aproximada trasera
		0.0f, 0.0f, -1.2f,                     // dirección hacia atrás
		1.0f, 0.0f, 0.0f,                     // atenuación
		20.0f);                                // ángulo
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		
		//Activar spotlight del cofre si está abierto
		if (mainWindow.getrotaCofre() < 0.0f) {
			glm::vec3 posCofre = glm::vec3(7.0f, 2.0f, -1.0f + mainWindow.getmueveCarro());
			glm::vec3 dirCofre = glm::vec3(0.0f, -0.3f, 1.0f);
			spotLights[1].SetFlash(posCofre, dirCofre);
		}
		else {
			spotLights[1].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f)); // Apagar luz si cofre está cerrado
		}

		//Encender/apagar faros del auto
		if (mainWindow.getBanderaDelantera() > 0.0f) {
			glm::vec3 posFrente = glm::vec3(7.5f, 0.0f, 0.0f + mainWindow.getmueveCarro());
			glm::vec3 dirFrente = glm::vec3(0.0f, 0.0f, 1.0f);
			spotLights[2].SetFlash(posFrente, dirFrente);
		}
		else {
			spotLights[2].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));
		}
		if (mainWindow.getBanderaTrasera() > 0.0f) {
			glm::vec3 posTrasera = glm::vec3(7.0f, 0.0f, -3.0f + mainWindow.getmueveCarro());
			glm::vec3 dirTrasera = glm::vec3(0.0f, 0.0f, -1.0f);
			spotLights[3].SetFlash(posTrasera, dirTrasera);
		}
		else {
			spotLights[3].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));
		}

		// Encender y apagar lámpara y fuego
		if (mainWindow.getValor() == 1.0f && mainWindow.getFuego() == 1.0f) {
			// Ambas luces encendidas
			pointLights[0] = pointLights[1]; // copia lámpara
			pointLights[1] = pointLights[2]; // copia fuego
			pointLightCount = 2;
		}
		else if (mainWindow.getValor() == 1.0f && mainWindow.getFuego() == 0.0f) {
			// Solo lámpara encendida
			pointLights[0] = pointLights[1]; // copia lámpara
			pointLightCount = 1;
		}
		else if (mainWindow.getValor() == 0.0f && mainWindow.getFuego() == 1.0f) {
			// Solo fuego encendido
			pointLights[0] = pointLights[2]; // copia fuego
			pointLightCount = 1;
		}
		else {
			// Ambas apagadas
			pointLightCount = 0;
		}

		// Actualizar las luces en el shader
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		//Auto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, -1.0f, -1.0 + mainWindow.getmueveCarro()));
		modelaux = model; //Se guarda la transformación del cuerpo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_M.RenderModel();
		
		//Llanta delantera derecha
		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Llanta de color negro
		model = glm::translate(model, glm::vec3(-2.3f, 1.0f, 3.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDD_M.RenderModel();		//Muestra Llanta
		
		//Llanta trasera derecha
		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Llanta de color negro
		model = glm::translate(model, glm::vec3(-2.37f, 1.0f, -4.15));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTD_M.RenderModel();		//Muestra Llanta

		//Lanta delantera izquierda
		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Llanta de color negro
		model = glm::translate(model, glm::vec3(2.2f, 1.0f, 3.65f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDI_M.RenderModel();		//Muestra Llanta

		//Llanta trasera izquierda
		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Llanta de color negro
		model = glm::translate(model, glm::vec3(2.27f, 1.0f, -4.15f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTI_M.RenderModel();		//Muestra Llanta

		//Cofre
		model = modelaux;
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Cofre de color verde oscuro
		model = glm::translate(model, glm::vec3(0.01f, 2.5f, 2.65f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();		//Muestra cofre

		shaderList[0].UseShader();
		// Restablecer el color y el material
		color = glm::vec3(1.0f, 1.0f, 1.0f);  // Color blanco por defecto
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		
		//Lampara texturizada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.5f, -1.0f, 5.0f)); // Posición adelante y a un lado del auto
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		//Fuego texturizado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, -1.0f, 5.0f)); // Posición adelante y a un lado del auto
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rota hacia el frente
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuego_M.RenderModel();

		//Octaedro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 8.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaedroTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmueveHelicoptero(), 5.0f, 6.0)); //movimiento del helicoptero adelante y atras
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

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}