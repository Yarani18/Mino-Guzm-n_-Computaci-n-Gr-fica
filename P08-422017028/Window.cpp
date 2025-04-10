#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	rotaLlantas = 0.0f;
	rotaCofre = 0.0f;
	mueveCarro = 0.0f;
	mueveHelicoptero = 0.0f;
	BanderaDelantera = 0.0f;
	BanderaTrasera = 0.0f;
	Valor = 0.0f;
	Fuego = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 08:Iluminacion 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	//Rotación para las llantas
	if (key == GLFW_KEY_B)
	{
		theWindow->rotaLlantas += 10.0;

	}
	if (key == GLFW_KEY_C)
	{
		theWindow->rotaLlantas -= 10.0;

	}
	//Rotación para el cofre
	if (key == GLFW_KEY_E)
	{
		if (theWindow->rotaCofre < -45)
		{
		}
		else
		{
			theWindow->rotaCofre -= 10.0;
		}

	}
	if (key == GLFW_KEY_F)
	{
		if (theWindow->rotaCofre == 0.0)
		{
		}
		else
		{
			theWindow->rotaCofre += 10.0;
		}

	}

	//Trasladar el carro
	// Mover coche hacia adelante (G)
	if (key == GLFW_KEY_G) {
		theWindow->mueveCarro += 0.5; 
		theWindow->rotaLlantas -= 10.0;

		// Activar faro delantero, desactivar trasero
		theWindow->BanderaDelantera = 1.0f;
		theWindow->BanderaTrasera = 0.0f;
	}

	// Mover coche hacia atrás (H)
	if (key == GLFW_KEY_H) {
		theWindow->mueveCarro -= 0.5; 
		theWindow->rotaLlantas += 10.0;

		// Activar faro trasero, desactivar delantero
		theWindow->BanderaDelantera = 0.0f;
		theWindow->BanderaTrasera = 1.0f;
	}


	//Trasladar el helicoptero
	if (key == GLFW_KEY_I)
	{
		theWindow->mueveHelicoptero -= 0.5;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->mueveHelicoptero += 0.5;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}
	//Encender y apagar la luz lampara
	if (key == GLFW_KEY_1)
	{
		theWindow->Valor = 0.0f;
	}

	if (key == GLFW_KEY_2)
	{
		theWindow->Valor = 1.0f;
	}
	
	//Encender y apagar la luz fuego
	if (key == GLFW_KEY_3)
	{
		theWindow->Fuego = 0.0f;
	}

	if (key == GLFW_KEY_4)
	{
		theWindow->Fuego = 1.0f;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
