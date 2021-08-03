#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"

using namespace std;

/*created a size values of triangle*/
GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

/*created array for color of triangle*/
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/*create vertex shader*/
const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

/*create fragment shader*/
const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";

/*making global variables for window size*/
int g_windowSizeX = 640;
int g_windowSizeY = 480;

/*making window size callback*/
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX/2, g_windowSizeY);
}

/*making window close if pressed Esc*/
void glfwKeyClickCallback (GLFWwindow* pWindow, int key, int scancode,int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    /*Glory to russia!*/
    setlocale(LC_ALL, "Russian");

    /*initialize the library */
    if (!glfwInit()) {
        cout << "GLFW didn't init" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "WoT2d", nullptr, nullptr);

    /*debug window*/
    if (!pWindow)
    {
        cout << "glfwCreateWindow failed. GLFW couldn't create window";
        glfwTerminate();
        return -1;
    }

    /*register window size in programm(line 46)*/
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);

    /*Register key Esc callback(line 53)*/
    glfwSetKeyCallback(pWindow, glfwKeyClickCallback);

    /*make the window's context current*/
    glfwMakeContextCurrent(pWindow);

    /*initialize GLAD*/
    if (!gladLoadGL()) {
        cout << "GLAD didn't load" << endl;
        return -1;
    }

    /*enter version of opengl in console*/
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

    glClearColor(0, 0, 0, 1);

    string vertex_Shader(vertex_shader);
    string fragmentShader(fragment_shader);
    Renderer::ShaderProgram shaderProgram(vertex_shader, fragment_shader);
    if (!shaderProgram.isCompiled())
    {
        cerr << "error:can't create shader program" << endl;
        return -1;
    }

    /*generated and connected buffer for vertexs*/
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    
    /*generated and connected buffer for colors*/
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    /*created vertex array*/
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /*enable vertex attrib array for vertexes*/
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /*enable vertex attrib array for colors*/
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /*loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /*render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*output traingle(finally)*/
        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
   
        /*swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /*poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}