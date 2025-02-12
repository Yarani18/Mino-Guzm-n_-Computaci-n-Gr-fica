//Mino Guzman Yara Amairani
#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;


// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
	gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 		\n\
}";

// Fragment Shader
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);	 		\n\
}";

void CrearTriangulo()
{
	GLfloat vertices[] = {
		//Primer triangulo
		-0.75f,0.25f,0.0f,
		-0.25f,0.25f,0.0f,
		-0.75f,-0.25f,0.0f,
		//Segundo triangulo
		-0.25f,0.25f,0.0f,
		-0.25f,-0.25f,0.0f,
		-0.75f,-0.25f,0.0f,
		//Tercer triangulo
		0.25f,0.0f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		//Cuarto triangulo
		0.75f,0.0f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1] = { shaderCode };
	theCode[0] = shaderCode;
	GLint codeLength[1]; //= { (GLint)strlen(shaderCode) };
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al compilar el shader %d: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al linkear: %s \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al validar: %s \n", eLog);
		return;
	}
}

// Función para actualizar el color de fondo cada 1 segundo
void updateBackgroundColor() {
	switch (colorIndex) {
	case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break; // Rojo
	case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break; // Verde
	case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break; // Azul
	}
	colorIndex = (colorIndex + 1) % 3; // Pasar al siguiente color
}

int main()
{
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ciclo de colores (RGB)", NULL, NULL);
	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);
	CrearTriangulo();
	CompileShaders();

	//cada vez que pasa un segundo, se llama a la funcion para cambia
	double lastTime = glfwGetTime(); // Tiempo de referencia para cambio de color
	
	int colorIndex = 0; //para alternar entre colores

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Verificar si ha pasado 1 segundo para cambiar de color
		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= 1.0) {
			updateBackgroundColor();
			lastTime = currentTime;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}
