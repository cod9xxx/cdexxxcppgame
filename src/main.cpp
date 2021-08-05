#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <glm/vec2.hpp>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

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

/*created points for texture*/
GLfloat texturePoints[] = {
    0.5f, 1.0f, 
    1.0f, 0.0f, 
    0.0f, 0.0f 
};

glm::ivec2 g_windowSize(640, 480);

/*making window size callback*/
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x /2, g_windowSize.y);
}

/*making window close if pressed Esc*/
void glfwKeyClickCallback (GLFWwindow* pWindow, int key, int scancode,int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    /*Glory to russia!*/
    setlocale(LC_ALL, "Russian");

    /*initialize the library */
    if (!glfwInit()) {
        cerr << "error:GLFW didn't init" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "WoT2d", nullptr, nullptr);

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
        cerr << "error:GLAD didn't load" << endl;
        return -1;
    }

    /*enter version of opengl in console*/
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

    glClearColor(0, 0, 0, 1);

    {
        /*added resourcemanager in main program*/
        ResourceManager resourceManager(argv[0]);
        auto pDeafultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDeafultShaderProgram)
        {
            cerr << "error:cant create shader program: " << "DefaultShader" << endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/texture_mario.png");

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

        GLuint texturePoints_vbo = 0;
        glGenBuffers(1, &texturePoints_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texturePoints_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texturePoints), texturePoints, GL_STATIC_DRAW);

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

        /*enable texture point array for texture*/
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texturePoints_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDeafultShaderProgram->use();
        pDeafultShaderProgram->setInt("tex", 0);

        /*loop until the user closes the window*/
        while (!glfwWindowShouldClose(pWindow))
        {
            /*render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /*output traingle(finally)*/
            pDeafultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /*swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /*poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}