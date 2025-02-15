// Inclusión de bibliotecas necesarias para OpenGL, entrada/salida y manejo de tiempo
#include <stdio.h>      // Para entrada/salida estándar
#include <string.h>     // Para manipulación de cadenas
#include <stdlib.h>     // Para funciones generales como srand()
#include <time.h>       // Para obtener semilla de números aleatorios
#include <glew.h>       // Biblioteca de extensiones de OpenGL
#include <glfw3.h>      // Biblioteca para crear ventanas y manejar contextos OpenGL

// Definición de constantes para el tamaño de la ventana
const int WIDTH = 800;  // Ancho de la ventana en píxeles
const int HEIGHT = 600; // Alto de la ventana en píxeles

// Variables globales para objetos OpenGL
GLuint VAO, VBO;    // Vertex Array Object y Vertex Buffer Object para almacenar geometría
GLuint shader;      // Identificador del programa de shader

// Shader de vértices: define cómo se posicionan los vértices en la pantalla
const char* vShader = R"(
    #version 330                           // Versión de GLSL
    layout (location = 0) in vec3 pos;     // Entrada de posición de vértices
    void main()
    {
        // Transformar coordenadas de objeto a coordenadas de clip
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    }
)";

// Shader de fragmentos: define el color de las letras
const char* fShader = R"(
    #version 330                           // Versión de GLSL
    out vec4 color;                        // Color de salida
    void main()
    {
        color = vec4(0.8, 0.3, 0.9, 1.0);  // Color púrpura para las letras
    }
)";

// Función para añadir y compilar un shader individual al programa
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    // Crear un nuevo shader del tipo especificado (vertex o fragment)
    GLuint theShader = glCreateShader(shaderType);

    // Preparar el código fuente del shader
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    // Obtener la longitud del código
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // Vincular el código fuente al shader
    glShaderSource(theShader, 1, theCode, codeLength);

    // Compilar el shader
    glCompileShader(theShader);

    // Verificar si la compilación fue exitosa
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    // Manejar errores de compilación
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compilando el %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    // Adjuntar el shader compilado al programa
    glAttachShader(theProgram, theShader);
}

// Función para crear y enlazar el programa de shader completo
void CompileShaders()
{
    // Crear un nuevo programa de shader
    shader = glCreateProgram();

    // Verificar creación exitosa del programa
    if (!shader)
    {
        printf("Error creando el programa de shader\n");
        return;
    }

    // Añadir shaders de vértices y fragmentos al programa
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

    // Manejar errores de validación
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validando el programa: '%s'\n", eLog);
        return;
    }
}

// Función para definir la geometría de las letras YMG
void CrearLetras()
{
    // Definir un array de vértices para dibujar las letras
    // Cada trio de vértices define un triángulo
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
        //Tríangulo 11
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
    // Enlazar el VAO para configuración
    glBindVertexArray(VAO);

    // Generar un Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    // Enlazar el VBO como buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copiar datos de vértices al buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar cómo interpretar los datos de vértices
    // 0 = índice del atributo, 3 = número de componentes, GL_FLOAT = tipo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Habilitar el atributo de vértices
    glEnableVertexAttribArray(0);

    // Desenlazar buffers para evitar modificaciones accidentales
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Función para cambiar el color de fondo de manera aleatoria
void updateBackgroundColor() {
    // Variable estática para evitar repetir el último color
    static int lastColorChoice = -1;
    int colorChoice;

    // Generar un color aleatorio diferente al último
    do {
        colorChoice = rand() % 3;  // 0: Rojo, 1: Verde, 2: Azul
    } while (colorChoice == lastColorChoice);

    // Guardar la elección actual
    lastColorChoice = colorChoice;

    // Establecer color de fondo según la elección
    switch (colorChoice) {
    case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break; // Rojo puro
    case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break; // Verde puro
    case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break; // Azul puro
    }
}

// Función principal del programa
int main()
{
    // Inicializar generador de números aleatorios con semilla de tiempo actual
    srand((unsigned)time(NULL));

    // Inicializar la biblioteca GLFW
    if (!glfwInit())
    {
        printf("Falló la inicialización de GLFW\n");
        return 1;
    }

    // Configurar versión y perfil de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Crear ventana con dimensiones y título
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Letras YMG Cuadradas", NULL, NULL);
    if (!mainWindow)
    {
        printf("Falló la creación de la ventana GLFW\n");
        glfwTerminate();
        return 1;
    }

    // Obtener tamaño del framebuffer
    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    // Establecer contexto OpenGL actual
    glfwMakeContextCurrent(mainWindow);

    // Inicializar GLEW para manejar extensiones de OpenGL
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Falló la inicialización de GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Configurar el área de renderizado
    glViewport(0, 0, BufferWidth, BufferHeight);

    // Crear geometría de letras y compilar shaders
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
        glDrawArrays(GL_TRIANGLES, 0, 75);  // 75 vértices en total
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