#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getrotaLlantas() { return rotaLlantas; }
	GLfloat getrotaCofre() { return rotaCofre; }
	GLfloat getmueveCarro() { return mueveCarro; }
	GLfloat getmueveHelicoptero() { return mueveHelicoptero; };
	GLfloat getValor() { return Valor; }
	GLfloat getBanderaDelantera() { return BanderaDelantera;  }
	GLfloat getBanderaTrasera() { return BanderaTrasera;  }
	GLfloat getFuego() { return Fuego;  }
	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz;
	GLfloat rotaLlantas, rotaCofre, mueveCarro;
	GLfloat mueveHelicoptero;
	GLfloat Valor;
	GLfloat BanderaDelantera;
	GLfloat BanderaTrasera;
	GLfloat Fuego;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

