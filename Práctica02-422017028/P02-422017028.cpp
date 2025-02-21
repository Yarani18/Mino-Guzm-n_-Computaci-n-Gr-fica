//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderRed = "shaders/shader_red.vert";
static const char* fShaderRed = "shaders/shader_red.frag";
static const char* vShaderGreen = "shaders/shader_green.vert";
static const char* fShaderGreen = "shaders/shader_green.frag";
static const char* vShaderBlue = "shaders/shader_blue.vert";
static const char* fShaderBlue = "shaders/shader_blue.frag";
static const char* vShaderBrown = "shaders/shader_brown.vert";
static const char* fShaderBrown = "shaders/shader_brown.frag";
static const char* vShaderDarkGreen = "shaders/shader_darkgreen.vert";
static const char* fShaderDarkGreen = "shaders/shader_darkgreen.frag";
float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
    unsigned int indices[] = {
        0,1,2,
        1,3,2,
        3,0,2,
        1,0,3
    };
    GLfloat vertices[] = {
        -0.5f, -0.5f,0.0f,    //0
        0.5f,-0.5f,0.0f,    //1
        0.0f,0.5f, -0.25f,    //2
        0.0f,-0.5f,-0.5f,    //3
    };
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
}

//Vértices de un cubo
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

void CrearLetrasyFiguras()
{
    //Letra Y (color lila)
    GLfloat vertices_letraY[] = {
        //Triangulo 1         RGB
        -0.7f,0.5f,0.5f,      0.8f,0.4f,0.9f,
        -0.6f,0.5f,0.5f,      0.8f,0.4f,0.9f,
        -0.6f,0.4f,0.5f,      0.8f,0.4f,0.9f,
        //Triangulo 2
        -0.6,0.5f,0.5f,       0.8f,0.4f,0.9f,
        -0.6,0.4f,0.5f,       0.8f,0.4f,0.9f,
        -0.5,0.4f,0.5f,       0.8f,0.4f,0.9f,
        //Triangulo 3
        -0.5,0.4,0.5f,        0.8f,0.4f,0.9f,
        -0.4,0.4f,0.5f,       0.8f,0.4f,0.9f,
        -0.4,0.5,0.5f,        0.8f,0.4f,0.9f,
        //Triangulo 4
        -0.4,0.4f,0.5f,       0.8f,0.4f,0.9f,
        -0.4,0.5,0.5f,        0.8f,0.4f,0.9f,
        -0.3,0.5f,0.5f,       0.8f,0.4f,0.9f,
        //Triangulo 5
        -0.6f,0.4f,0.5f,      0.8f,0.4f,0.9f,
        -0.4,0.4f,0.5f,       0.8f,0.4f,0.9f,
        -0.45,0.35f,0.5f,     0.8f,0.4f,0.9f,
        //Triangulo 6
        -0.6f,0.4f,0.5f,      0.8f,0.4f,0.9f,
        -0.45f,0.35f,0.5f,    0.8f,0.4f,0.9f,
        -0.55,0.35f,0.5f,     0.8f,0.4f,0.9f,
        //Triangulo 7
        -0.45f,0.35f,0.5f,    0.8f,0.4f,0.9f,
        -0.55,0.35f,0.5f,     0.8f,0.4f,0.9f,
        -0.45f,0.0f,0.5f,     0.8f,0.4f,0.9f,
        //Triangulo 8
        -0.55f,0.35f,0.5f,    0.8f,0.4f,0.9f,
        -0.45f,0.0f,0.5f,     0.8f,0.4f,0.9f,
        -0.55f,0.0f,0.5f,     0.8f,0.4f,0.9f,
    };
    MeshColor* letraY = new MeshColor();
    letraY->CreateMeshColor(vertices_letraY, 144); // 24 vértices * 6 valores por vértice
    meshColorList.push_back(letraY);

    //Letra M (rosa claro)
    GLfloat vertices_letraM[] = {
        // Triangulo 9          RGB
        -0.2f,0.0f,0.3f,      1.0f,0.7f,0.8f,
        -0.2f,0.5f,0.3f,       1.0f,0.7f,0.8f,
        -0.1,0.0f,0.3f,       1.0f,0.7f,0.8f,
        //Triangulo 10
        -0.2f,0.5f,0.3f,      1.0f,0.7f,0.8f,
        -0.1f,0.0f,0.3f,      1.0f,0.7f,0.8f,
        -0.1f,0.5f,0.3f,      1.0f,0.7f,0.8f,
        //Tríangulo 11
        -0.1f,0.5f,0.3f,      1.0f,0.7f,0.8f,
        -0.1,0.35f,0.3f,      1.0f,0.7f,0.8f,
        0.0f,0.35f,0.3f,      1.0f,0.7f,0.8f,
        //Triangulo 12
        -0.1f,0.35f,0.3f,     1.0f,0.7f,0.8f,
        0.0f,0.25f,0.3f,      1.0f,0.7f,0.8f,
        0.1f,0.35f,0.3f,      1.0f,0.7f,0.8f,
        //Triangulo 13
        0.0f,0.35f,0.3f,      1.0f,0.7f,0.8f,
        0.1f,0.35f,0.3f,      1.0f,0.7f,0.8f,
        0.1f,0.5f,0.3f,       1.0f,0.7f,0.8f,
        //Triangulo 14
        0.1f,0.5f,0.3f,       1.0f,0.7f,0.8f,
        0.2f,0.5f,0.3f,       1.0f,0.7f,0.8f,
        0.1f,0.0f,0.3f,       1.0f,0.7f,0.8f,
        //Triangulo 15
        0.2f,0.5f,0.3f,       1.0f,0.7f,0.8f,
        0.1f,0.0f,0.3f,       1.0f,0.7f,0.8f,
        0.2f,0.0f,0.3f,       1.0f,0.7f,0.8f,
    };
    MeshColor* letraM = new MeshColor();
    letraM->CreateMeshColor(vertices_letraM, 126); // 21 vértices * 6 valores por vértice
    meshColorList.push_back(letraM);

    //Letra G (color naranja)
    GLfloat vertices_letraG[] = {
        //Triangulo 16       RGB
        0.3f,0.5f,0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.5f,0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 17
        0.7f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        0.3f,0.5f,0.1f,     1.0f,0.6f,0.0f,
        0.3f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 18
        0.3f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        0.4f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        0.3f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 19
        0.4f,0.4f,0.1f,     1.0f,0.6f,0.0f,
        0.3f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        0.4f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 20
        0.4f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        0.4f,0.1f,0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.1f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 21
        0.4f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.0f,0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.1f,0.1f,     1.0f,0.6f,0.0f,
        //Triangulo 22
        0.7f,0.1f,0.1f,     1.0f,0.6f,0.0f,
        0.6f,0.1f,-0.1f,     1.0f,0.6f,0.0f,
        0.7f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        //Triangulo 23
        0.6f,0.1f,-0.1f,     1.0f,0.6f,0.0f,
        0.6f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        0.7f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        //Triangulo 24
        0.6f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        0.6f,0.19f,-0.1f,    1.0f,0.6f,0.0f,
        0.5f,0.19f,-0.1f,    1.0f,0.6f,0.0f,
        //Triangulo 25
        0.6f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        0.5f,0.25f,-0.1f,    1.0f,0.6f,0.0f,
        0.5f,0.19f,-0.1f,    1.0f,0.6f,0.0f
    };
    MeshColor* letraG = new MeshColor();
    letraG->CreateMeshColor(vertices_letraG, 180); // 30 vértices * 6 valores por vértice
    meshColorList.push_back(letraG);  
    GLfloat vertices_letras[] = {
        //X            Y            Z            R        G        B
        -1.0f,      -1.0f,      0.5f,         0.0f,    0.0f,    1.0f,
        1.0f,       -1.0f,      0.5f,         0.0f,    0.0f,    1.0f,
        1.0f,       1.0f,       0.5f,         0.0f,    0.0f,    1.0f,
    };
    MeshColor* letras = new MeshColor();
    letras->CreateMeshColor(vertices_letras, 18);
    meshColorList.push_back(letras);
    /*
    // Triángulo azul
    GLfloat vertices_trianguloazul[] = {
        //X            Y            Z            R        G        B
        -1.0f,      -1.0f,      0.5f,         0.0f,    0.0f,    1.0f,
        1.0f,       -1.0f,      0.5f,         0.0f,    0.0f,    1.0f,
        0.0f,       1.0f,       0.5f,         0.0f,    0.0f,    1.0f,
    };
    MeshColor* trianguloazul = new MeshColor();
    trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
    meshColorList.push_back(trianguloazul);

    // Triángulo verde (trasladado en Y)
    GLfloat vertices_trianguloverde[] = {
        //X            Y            Z            R        G        B
        -1.0f,      -0.5f,      0.5f,         0.0f,    0.5f,    0.0f,
        1.0f,       -0.5f,      0.5f,         0.0f,    0.5f,    0.0f,
        0.0f,       1.5f,       0.5f,         0.0f,    0.5f,    0.0f,
    };
    MeshColor* trianguloverde = new MeshColor();
    trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
    meshColorList.push_back(trianguloverde);

    // Cuadrado rojo
    GLfloat vertices_cuadradorojo[] = {
        //X            Y            Z            R        G        B
        -0.5f,      -0.5f,      0.5f,         1.0f,    0.0f,    0.0f,
        0.5f,       -0.5f,      0.5f,         1.0f,    0.0f,    0.0f,
        0.5f,       0.5f,       0.5f,         1.0f,    0.0f,    0.0f,
        -0.5f,      -0.5f,      0.5f,         1.0f,    0.0f,    0.0f,
        0.5f,       0.5f,       0.5f,         1.0f,    0.0f,    0.0f,
        -0.5f,      0.5f,       0.5f,         1.0f,    0.0f,    0.0f,
    };
    MeshColor* cuadradorojo = new MeshColor();
    cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
    meshColorList.push_back(cuadradorojo);

    // Cuadrado verde
    GLfloat vertices_cuadradoverde[] = {
        //X            Y            Z            R        G        B
        -0.5f,      -0.5f,      0.5f,         0.0f,    1.0f,    0.0f,
        0.5f,       -0.5f,      0.5f,         0.0f,    1.0f,    0.0f,
        0.5f,       0.5f,       0.5f,         0.0f,    1.0f,    0.0f,
        -0.5f,      -0.5f,      0.5f,         0.0f,    1.0f,    0.0f,
        0.5f,       0.5f,       0.5f,         0.0f,    1.0f,    0.0f,
        -0.5f,      0.5f,       0.5f,         0.0f,    1.0f,    0.0f,
    };
    MeshColor* cuadradoverde = new MeshColor();
    cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
    meshColorList.push_back(cuadradoverde);

    // Cuadrado café
    GLfloat vertices_cuadradocafe[] = {
        //X            Y            Z            R        G        B
        -0.5f,      -0.5f,      0.5f,         0.478f,  0.255f,  0.067f,
        0.5f,       -0.5f,      0.5f,         0.478f,  0.255f,  0.067f,
        0.5f,       0.5f,       0.5f,         0.478f,  0.255f,  0.067f,
        -0.5f,      -0.5f,      0.5f,         0.478f,  0.255f,  0.067f,
        0.5f,       0.5f,       0.5f,         0.478f,  0.255f,  0.067f,
        -0.5f,      0.5f,       0.5f,         0.478f,  0.255f,  0.067f,
    };
    MeshColor* cuadradocafe = new MeshColor();
    cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
    meshColorList.push_back(cuadradocafe);
    */
}
void CreateShaders()
{
    Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
    shader2->CreateFromFiles(vShaderColor, fShaderColor);
    shaderList.push_back(*shader2);

    // Shader rojo para la casa
    Shader* shaderRed = new Shader();
    shaderRed->CreateFromFiles(vShaderRed, fShaderRed);
    shaderList.push_back(*shaderRed);

    // Shader verde para ventanas y puerta
    Shader* shaderGreen = new Shader();
    shaderGreen->CreateFromFiles(vShaderGreen, fShaderGreen);
    shaderList.push_back(*shaderGreen);

    // Shader azul para el techo
    Shader* shaderBlue = new Shader();
    shaderBlue->CreateFromFiles(vShaderBlue, fShaderBlue);
    shaderList.push_back(*shaderBlue);

    // Shader café para troncos de árboles
    Shader* shaderBrown = new Shader();
    shaderBrown->CreateFromFiles(vShaderBrown, fShaderBrown);
    shaderList.push_back(*shaderBrown);

    // Shader verde oscuro para copas de árboles
    Shader* shaderDarkGreen = new Shader();
    shaderDarkGreen->CreateFromFiles(vShaderDarkGreen, fShaderDarkGreen);
    shaderList.push_back(*shaderDarkGreen);
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();
    CreaPiramide(); //índice 0 en MeshList
    CrearCubo();//índice 1 en MeshList
    CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
    CreateShaders();
    glEnable(GL_DEPTH_TEST); 
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    //Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
    //glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    //Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
    glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

    //Loop mientras no se cierra la ventana
    while (!mainWindow.getShouldClose())
    {
        //Recibir eventos del usuario
        glfwPollEvents();
        //Limpiar la ventana
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
       
        //Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        /*
        //Renderizar letra Y (lila)
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[0]->RenderMeshColor();

        //Renderizar letra M (rosa claro)
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[1]->RenderMeshColor();

        //Renderizar letra G (naranja)
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[2]->RenderMeshColor();
       
       // Renderizar triángulo azul
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-8.0f, 0.0f, -4.0f));  // Más separado en X
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Hacerlo más grande
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[0]->RenderMeshColor();

        // Renderizar triángulo verde
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -4.0f));  // Nueva posición
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Hacerlo más grande
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[1]->RenderMeshColor();

        // Renderizar cuadrado rojo
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));   // Centro
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));  // Hacerlo más grande
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[2]->RenderMeshColor();

        // Renderizar cuadrado verde
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, -4.0f));   // Nueva posición
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // Hacerlo más grande
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[3]->RenderMeshColor();

        // Renderizar cuadrado café
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(8.0f, 0.0f, -4.0f));   // Nueva posición
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Hacerlo más grande
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[4]->RenderMeshColor();
        */
       
       //Renderización de la casa
       // En el main loop:
        // Casa principal (cubo rojo)
        shaderList[2].useShader(); // shader rojo
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para la casa

        // Techo (pirámide azul)
        shaderList[4].useShader(); // shader azul
        uniformModel = shaderList[4].getModelLocation();
        uniformProjection = shaderList[4].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 3.8f, -15.0f));
        model = glm::scale(model, glm::vec3(6.0f, 3.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh(); // Usar pirámide para el techo

        // Ventana izquierda (cubo verde)
        shaderList[3].useShader(); // shader verde
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.7f, 0.7f, -10.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para ventanas

        // Ventana derecha (cubo verde)
        shaderList[3].useShader(); // shader verde
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.7f, 0.7f, -10.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para ventanas

        // Puerta (cubo verde)
        shaderList[3].useShader(); // shader verde
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -10.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para la puerta

        // Árbol izquierdo
        // Tronco (cubo café)
        shaderList[5].useShader();
        uniformModel = shaderList[5].getModelLocation();
        uniformProjection = shaderList[5].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-4.0f, -1.5f, -15.0f));
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para el tronco

        // Copa (pirámide verde oscuro)
        shaderList[6].useShader();
        uniformModel = shaderList[6].getModelLocation();
        uniformProjection = shaderList[6].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-4.0f, -0.1f, -15.0f));
        model = glm::scale(model, glm::vec3(2.3f, 2.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh(); // Usar pirámide para la copa

        // Árbol derecho
        // Tronco (cubo café)
        shaderList[5].useShader();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(4.0f, -1.5f, -15.0f));
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // Usar cubo para el tronco

        // Copa (pirámide verde oscuro)
        shaderList[6].useShader();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(4.0f, -0.1f, -15.0f));
        model = glm::scale(model, glm::vec3(2.3f, 2.0f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh(); // Usar pirámide para la copa
        
        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
moel = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/