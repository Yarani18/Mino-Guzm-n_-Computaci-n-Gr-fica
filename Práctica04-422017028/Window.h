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
    GLfloat getarticulacion1() { return articulacion1; }
    GLfloat getarticulacion2() { return articulacion2; }
    GLfloat getarticulacion3() { return articulacion3; }
    GLfloat getarticulacion4() { return articulacion4; }
    GLfloat getarticulacion5() { return articulacion5; }
    GLfloat getarticulacion6() { return articulacion6; }
    // Métodos getter para las llantas
    GLfloat getllanta1() { return llanta1; }
    GLfloat getllanta2() { return llanta2; }
    GLfloat getllanta3() { return llanta3; }
    GLfloat getllanta4() { return llanta4; }
    //Método getter para las articulaciones de la cola
    GLfloat getArticulacionCola() { return ArticulacionCola; }
    GLfloat getArticulacionCola1() { return ArticulacionCola1; }
    GLfloat getArticulacionCola2() { return ArticulacionCola2; }
    //Método getter para las articulaciones de las patas
    GLfloat getArticulacionPataDI() { return articulacionPataDI; }
    GLfloat getArticulacionPataDI_inf() { return articulacionPataDI_inf; }
    GLfloat getArticulacionPataDD() { return articulacionPataDD; }
    GLfloat getArticulacionPataDD_inf() { return articulacionPataDD_inf; }
    GLfloat getArticulacionPataPI() { return articulacionPataPI; }
    GLfloat getArticulacionPataPI_inf() { return articulacionPataPI_inf; }
    GLfloat getArticulacionPataPD() { return articulacionPataPD; }
    GLfloat getArticulacionPataPD_inf() { return articulacionPataPD_inf; }

    ~Window();
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;
    // Variables para las rotaciones de las llantas
    GLfloat llanta1, llanta2, llanta3, llanta4;
    //Variables para las rotaciones de las articulaciones de la cola
    GLfloat ArticulacionCola, ArticulacionCola1, ArticulacionCola2;
    //Variables para las rotaciones de las articulaciones de la pata
    GLfloat articulacionPataDI, articulacionPataDI_inf, articulacionPataDD, articulacionPataDD_inf, articulacionPataPI, articulacionPataPI_inf, articulacionPataPD, articulacionPataPD_inf;

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