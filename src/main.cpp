#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

using namespace std;

/*Making global variables for window size*/
int g_windowSizeX = 640;
int g_windowSizeY = 480;

/*Making window size callback*/
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX/2, g_windowSizeY);
}

/*Making window close if pressed Esc*/
void glfwKeyClickCallback (GLFWwindow* pWindow, int key, int scancode,int action, int mode) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

}

int main(void)
{
    setlocale(LC_ALL, "Russian");

    /* Initialize the library */
    if (!glfwInit()) {
        cout << "GLFW didn't init" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "WoT2d", nullptr, nullptr);

    /*Debug window*/
    if (!pWindow)
    {
        cout << "glfwCreateWindow failed. GLFW couldn't create window";
        glfwTerminate();
        return -1;
    }

    /*Register window size in programm*/
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);

    /*Register key Esc callback*/
    glfwSetKeyCallback(pWindow, glfwKeyClickCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    /*Initialize GLAD*/
    if (!gladLoadGL()) {
        cout << "GLAD didn't load" << endl;
        return -1;
    }

    glClearColor(1, 0, 0, 1);

    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}