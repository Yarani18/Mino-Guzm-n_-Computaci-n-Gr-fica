// Inclusi�n de bibliotecas necesarias para OpenGL, entrada/salida y manejo de tiempo
#include <stdio.h>      // Para entrada/salida est�ndar
#include <string.h>     // Para manipulaci�n de cadenas
#include <stdlib.h>     // Para funciones generales como srand()
#include <time.h>       // Para obtener semilla de n�meros aleatorios
#include <glew.h>       // Biblioteca de extensiones de OpenGL
#include <glfw3.h>      // Biblioteca para crear ventanas y manejar contextos OpenGL

// Definici�n de constantes para el tama�o de la ventana
const int WIDTH = 800;  // Ancho de la ventana en p�xeles
const int HEIGHT = 600; // Alto de la ventana en p�xeles

// Variables globales para objetos OpenGL
GLuint VAO, VBO;    // Vertex Array Object y Vertex Buffer Object para almacenar geometr�a
GLuint shader;      // Identificador del programa de shader

// Shader de v�rtices: define c�mo se posicionan los v�rtices en la pantalla
const char* vShader = R"(
    #version 330                           // Versi�n de GLSL
    layout (location = 0) in vec3 pos;     // Entrada de posici�n de v�rtices
    void main()
    {
        // Transformar coordenadas de objeto a coordenadas de clip
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    }
)";

// Shader de fragmentos: define el color de las letras
const char* fShader = R"(
    #version 330                           // Versi�n de GLSL
    out vec4 color;                        // Color de salida
    void main()
    {
        color = vec4(0.8, 0.3, 0.9, 1.0);  // Color p�rpura para las letras
    }
)";

// Funci�n para a�adir y compilar un shader individual al programa
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    // Crear un nuevo shader del tipo especificado (vertex o fragment)
    GLuint theShader = glCreateShader(shaderType);

    // Preparar el c�digo fuente del shader
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    // Obtener la longitud del c�digo
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // Vincular el c�digo fuente al shader
    glShaderSource(theShader, 1, theCode, codeLength);

    // Compilar el shader
    glCompileShader(theShader);

    // Verificar si la compilaci�n fue exitosa
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    // Manejar errores de compilaci�n
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compilando el %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    // Adjuntar el shader compilado al programa
    glAttachShader(theProgram, theShader);
}

// Funci�n para crear y enlazar el programa de shader completo
void CompileShaders()
{
    // Crear un nuevo programa de shader
    shader = glCreateProgram();

    // Verificar creaci�n exitosa del programa
    if (!shader)
    {
        printf("Error creando el programa de shader\n");
        return;
    }

    // A�adir shaders de v�rtices y fragmentos al programa
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    // Variables para verificar resultados de enlace
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Enlazar el programa de shader
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    // Manejar errores de enlace
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error enlazando el programa: '%s'\n", eLog);
        return;
    }

    // Validar el programa de shader
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    // Manejar errores de validaci�n
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validando el programa: '%s'\n", eLog);
        return;
    }
}

// Funci�n para definir la geometr�a de las letras YMG
void CrearLetras()
{
    // Definir un array de v�rtices para dibujar las letras
    // Cada trio de v�rtices define un tri�ngulo
    // Las coordenadas van de -1.0 a 1.0 en el plano x,y
    GLfloat vertices[] = {
        // Y
        //Triangulo 1
        -0.7f,0.5f,0.0f,
        -0.6f,0.5f,0.0f,
        -0.6f,0.4f,0.0f,
        //Triangulo 2
        -0.6,0.5f,0.0f,
        -0.6,0.4f,0.0f,
        -0.5,0.4f,0.0f,
        //Triangulo 3
        -0.5,0.4,0.0f,
        -0.4,0.4f,0.0f,
        -0.4,0.5,0.0f,
        //Triangulo 4
        -0.4,0.4f,0.0f,
        -0.4,0.5,0.0f,
        -0.3,0.5f,0.0f,
        //Triangulo 5
        -0.6f,0.4f,0.0f,
        -0.4,0.4f,0.0f,
        -0.45,0.35f,0.0f,
        //Triangulo 6
        -0.6f,0.4f,0.0f,
        -0.45f,0.35f,0.0f,
        -0.55,0.35f,0.0f,
        //Triangulo 7
        -0.45f,0.35f,0.0f,
        -0.55,0.35f,0.0f,
        -0.45f,0.0f,0.0f,
        //Triangulo 8
        -0.55f,0.35f,0.0f,
        -0.45f,0.0f,0.0f,
        -0.55f,0.0f,0.0f,
        //M
        // Triangulo 9
        -0.2f,0.0f,0.0f,
        -0.2f,0.5,0.0f,
        -0.1,0.0f,0.0f,
        //Triangulo 10
        -0.2f,0.5f,0.0f,
        -0.1f,0.0f,0.0f,
        -0.1f,0.5f,0.0f,
        //Tr�angulo 11
        -0.1f,0.5f,0.0f,
        -0.1,0.35f,0.0f,
        0.0f,0.35f,0.0f,
        //Triangulo 12
        -0.1f,0.35f,0.0f,
        0.0f,0.25f,0.0f,
        0.1f,0.35f,0.0f,
        //Triangulo 13
        0.0f,0.35f,0.0f,
        0.1f,0.35f,0.0f,
        0.1f,0.5f,0.0f,
        //Triangulo 14
        0.1f,0.5f,0.0f,
        0.2f,0.5f,0.0f,
        0.1f,0.0f,0.0f,
        //Triangulo 15
        0.2f,0.5f,0.0f,
        0.1f,0.0f,0.0f,
        0.2f,0.0f,0.0f,
        // G
        //Triangulo 16
        0.3f, 0.5f, 0.0f,		
        0.7f, 0.5f, 0.0f,
        0.7f, 0.4f, 0.0f,
        //Triangulo 17
        0.7f, 0.4f, 0.0f,		
        0.3f, 0.5f, 0.0f,
        0.3f, 0.4f, 0.0f,
        //Triangulo 18
        0.3f, 0.4f, 0.0f,		
        0.4f, 0.4f, 0.0f,
        0.3f, 0.0f, 0.0f,
        //Triangulo 19
        0.4f, 0.4f, 0.0f,		
        0.3f, 0.0f, 0.0f,
        0.4f, 0.0f, 0.0f,
        //Triangulo 20
        0.4f, 0.0f, 0.0f,		
        0.4f, 0.1f, 0.0f,
        0.7f, 0.1f, 0.0f,
        //Triangulo 21
        0.4f, 0.0f, 0.0f,		
        0.7f, 0.0f, 0.0f,
        0.7f, 0.1f, 0.0f,
        //Triangulo 22
        0.7f, 0.1f, 0.0f,		
        0.6f, 0.1f, 0.0f,
        0.7f, 0.25f, 0.0f,
        //Triangulo 23
        0.6f, 0.1f, 0.0f,		
        0.6f, 0.25f, 0.0f,
        0.7f, 0.25f, 0.0f,
        //Triangulo 24
        0.6f, 0.25f, 0.0f,		
        0.6f, 0.19f, 0.0f,
        0.5f, 0.19f, 0.0f,
        //Triangulo 25
        0.6f, 0.25f, 0.0f,		
        0.5f, 0.25f, 0.0f,
        0.5f, 0.19f, 0.0f
    };

    // Generar un Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    // Enlazar el VAO para configuraci�n
    glBindVertexArray(VAO);

    // Generar un Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    // Enlazar el VBO como buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copiar datos de v�rtices al buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar c�mo interpretar los datos de v�rtices
    // 0 = �ndice del atributo, 3 = n�mero de componentes, GL_FLOAT = tipo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Habilitar el atributo de v�rtices
    glEnableVertexAttribArray(0);

    // Desenlazar buffers para evitar modificaciones accidentales
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Funci�n para cambiar el color de fondo de manera aleatoria
void updateBackgroundColor() {
    // Variable est�tica para evitar repetir el �ltimo color
    static int lastColorChoice = -1;
    int colorChoice;

    // Generar un color aleatorio diferente al �ltimo
    do {
        colorChoice = rand() % 3;  // 0: Rojo, 1: Verde, 2: Azul
    } while (colorChoice == lastColorChoice);

    // Guardar la elecci�n actual
    lastColorChoice = colorChoice;

    // Establecer color de fondo seg�n la elecci�n
    switch (colorChoice) {
    case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break; // Rojo puro
    case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break; // Verde puro
    case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break; // Azul puro
    }
}

// Funci�n principal del programa
int main()
{
    // Inicializar generador de n�meros aleatorios con semilla de tiempo actual
    srand((unsigned)time(NULL));

    // Inicializar la biblioteca GLFW
    if (!glfwInit())
    {
        printf("Fall� la inicializaci�n de GLFW\n");
        return 1;
    }

    // Configurar versi�n y perfil de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Crear ventana con dimensiones y t�tulo
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Letras YMG Cuadradas", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fall� la creaci�n de la ventana GLFW\n");
        glfwTerminate();
        return 1;
    }

    // Obtener tama�o del framebuffer
    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    // Establecer contexto OpenGL actual
    glfwMakeContextCurrent(mainWindow);

    // Inicializar GLEW para manejar extensiones de OpenGL
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Fall� la inicializaci�n de GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Configurar el �rea de renderizado
    glViewport(0, 0, BufferWidth, BufferHeight);

    // Crear geometr�a de letras y compilar shaders
    CrearLetras();
    CompileShaders();

    // Establecer color de fondo inicial aleatorio
    updateBackgroundColor();

    // Variables para control de tiempo de cambio de color
    double initialTime = glfwGetTime();
    double nextColorChangeTime = initialTime + 2.0;

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Obtener tiempo actual
        double currentTime = glfwGetTime();

        // Cambiar color cada 2 segundos
        if (currentTime >= nextColorChangeTime) {
            updateBackgroundColor();
            nextColorChangeTime = currentTime + 2.0;
        }

        // Procesar eventos de ventana
        glfwPollEvents();

        // Limpiar buffer de color con color de fondo actual
        glClear(GL_COLOR_BUFFER_BIT);

        // Activar programa de shader
        glUseProgram(shader);

        // Dibujar las letras
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 75);  // 75 v�rtices en total
        glBindVertexArray(0);
        glUseProgram(0);

        // Intercambiar buffers de pantalla
        glfwSwapBuffers(mainWindow);
    }

    // Limpiar recursos de OpenGL y GLFW
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}