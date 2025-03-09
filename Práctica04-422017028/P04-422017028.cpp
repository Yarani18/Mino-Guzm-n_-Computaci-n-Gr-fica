/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}



void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(20, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 6.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
	glm::mat4 modelaux2(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
	glm::mat4 modelaux3(1.0);//Inicializar matriz de modelo 4x4 auxiliar para la jerarquía
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f);	//Color púrpura para el fondo de la grúa
		//glClearColor(1.0f, 0.5f, 0.0f, 0.0f); //Color naranja para el fondo del gato
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//AQUÍ SE DIBUJA LA CABINA, LA BASE, LAS 4 LLANTAS
		//Cabina	(Cubo)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, 4.0f, -4.0f));
		modelaux = model;
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f);		//Color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		model = modelaux;

		//Base de la cabina (Piramide cuadrangular)
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(8.0f, 1.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);    //Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMeshGeometry();	//dibuja piramide cuadrangular
		model = modelaux;

		//Llantas (4 cilindros)
		// Llanta frontal izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.5f, -1.5f, 2.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Orientación horizontal
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta1()), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la llanta 1
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f); // Negro para las llantas
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la llanta

		// Llanta frontal derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -1.5f, 2.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Orientación horizontal
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta2()), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la llanta 2
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la llanta

		// Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.5f, -1.5f, -2.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Orientación horizontal
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta3()), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la llanta 3
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la llanta

		// Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -1.5f, -2.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Orientación horizontal
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta4()), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la llanta 4
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la llantas

		//BRAZO
		//articulación 1 (Esfera)
		//rotación alrededor de la articulación que une con la cabina
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;                             		//matriz secundario auxiliar temporal para que no se herede la escala
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 0.0f);    //Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();	//dibuja esfera
		model = modelaux;

		//primer brazo que conecta con la cabina (Cubo)
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 1.0f);	//Color azul claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo
		model = modelaux;

		//articulación 2 (Esfera)
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//dibujar una pequeña esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);    //Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();	//dibuja esfera
		model = modelaux;

		//Segundo brazo (Cubo)
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5f, 0.0f);	//Color naranja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo
		model = modelaux;

		//articulación 3 extremo derecho del segundo brazo (esfera)
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//dibujar una pequeña esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);    //Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();//dibuja esfera
		model = modelaux;

		//Brazo 3 (Cubo)
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);		//Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo
		model = modelaux;

		//articulacion 4 (esfera)
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));   //Para girar la cabina en el eje x inicial
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		//dibujar una pequeña esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);    //Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();	//dibuja esfera

		model = modelaux;

		//Canasta (Cubo)
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);	//Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo

/*
		//--------------------GATO--------------------
		//CUERPO PRINCIPAL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model;
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.5f, 0.5f, 0.5f); //Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		// CUELLO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 0.0f)); // Posición entre el cuerpo y la cabeza
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación para orientarlo hacia la cabeza
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.35f, 2.0f, 0.35f)); // Cuello más largo (aumentado de 1.2f a 2.0f en X)
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// CABEZA (ESFERA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		modelaux = model; // Guardar matriz para orejas y visor
		modelaux3 = modelaux;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//OREJA IZQUIERDA (CONO)
		model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, 0.8f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry(); //dibuja Cono
		model = modelaux;

		//OREJA DERECHA (CONO)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.6f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry(); //dibuja Cono
		model = modelaux2;

		//COLA ARTICULADA

		//Primera articulacion (base de la cola)
		//Esfera de la primera articulacion
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionCola()), glm::vec3(0.0f, 0.0f, 1.0f)); // Usar el valor variable
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		model = modelaux;

		//Primer segmento de la cola
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.2f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro
		model = modelaux;

		//Esfera de la segunda articulacion
		model = glm::translate(model, glm::vec3(1.4f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionCola1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Usar el valor variable
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		model = modelaux;

		//Segundo segmento de la cola
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro
		model = modelaux;

		//Esfera de la tercer articulacion
		model = glm::translate(model, glm::vec3(1.1f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionCola2()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada por tecla
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Dibuja esfera
		model = modelaux;

		//Tercer segmento de la cola
		model = glm::translate(model, glm::vec3(0.6f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro
		model = modelaux3;

		//OJOS
		// OJO IZQUIERDO
		model = glm::translate(model, glm::vec3(-0.7f, 0.7f, 0.6f)); // Hacia el lado izquierdo de la cara
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // Tamaño proporcional
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Dibuja esfera para el ojo


		// PUPILA IZQUIERDA
		model = glm::translate(model, glm::vec3(-0.5f, 0.6f, 0.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Dibuja esfera para la pupila

		// OJO DERECHO
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-0.7f, 0.7f, -0.6f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Dibuja esfera para el ojo

		// PUPILA DERECHA
		model = glm::translate(model, glm::vec3(-0.5f, 0.6f, -0.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// NARIZ
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-1.35f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Usar esfera para una nariz redonda

		// BOCA
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-1.4f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.2f, 0.3f); // Rosa intenso
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); // Usar cubo para la boca

		// SONRISA LINEAL
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-1.42f, -0.05f, 0.0f)); // Ligeramente más adelante que la boca
		model = glm::scale(model, glm::vec3(0.05f, 0.03f, 0.6f)); // Línea delgada pero ancha
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro para la línea
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); // Usar cubo para hacer una línea recta

		// ESQUINA IZQUIERDA CURVADA
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-1.42f, 0.0f, 0.35f)); // Extremo izquierdo
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.1f)); // Pequeño punto para la curvatura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Usar esfera para punto redondeado

		// ESQUINA DERECHA CURVADA
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-1.42f, 0.0f, -0.35f)); // Extremo derecho
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.1f)); // Pequeño punto para la curvatura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Usar esfera para punto redondeado
		model = modelaux2;

		//PATA DELANTERA IZQUIERDA
		//Articulacion superior
		model = glm::translate(model, glm::vec3(-1.7f, -1.0f, 0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataDI()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		//Segmento superior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		//Articulacion inferior
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataDI_inf()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		//Segmento inferior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		//Esfera
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// PATA TRASERA IZQUIERDA
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.7f, -1.0f, 0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataPI_inf()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento superior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f)); // Hacia abajo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		// Articulación inferior (rodilla)
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f)); // Fin del segmento superior
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataPI()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento inferior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Hacia abajo
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		//Esfera
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Posición en la esquina del cuerpo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux2;

		//PATA DELANTERA DERECHA
		model = glm::translate(model, glm::vec3(-1.7f, -1.0f, -0.8f)); // Posición en la esquina del cuerpo
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataDD()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento superior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f)); // Hacia abajo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		// Articulación inferior (rodilla)
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f)); // Fin del segmento superior
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataDD_inf()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento inferior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Hacia abajo
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		//Esfera
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Posición en la esquina del cuerpo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica

		//PATA TRASERA DERECHA
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.5f, -1.0f, -0.8f)); // Posición en la esquina trasera derecha del cuerpo
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataPD()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento superior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f)); // Hacia abajo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		// Articulación inferior (rodilla)
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f)); // Fin del segmento superior
		model = glm::rotate(model, glm::radians(mainWindow.getArticulacionPataPD_inf()), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación controlada
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
		model = modelaux;

		// Segmento inferior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Hacia abajo
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); // Cilindro para la pata
		model = modelaux;

		//Esfera
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // Posición en la esquina del cuerpo
		modelaux = model; // Guardar para la articulación inferior
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); // Articulación esférica
*/
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}