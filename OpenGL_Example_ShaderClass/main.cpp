///This program is a simple rectangle in screen using ModernOpenGL
///Author: Jorge Andres Gonzalez Borboa
///Following: https://learnopengl.com/#!Getting-started/Shaders

///You need GLFW||GLUT||freeGLUT and GLEW||GLAD to compile this example.

#define GLEW_STATIC

/// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <cassert>
#include <stdexcept>

#include <iostream>

using namespace std;

#include "Shader.h"

///Data for the square
///we are drawing
GLfloat vertices[] =
{
     //Position          //Color
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,      // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,      // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,      // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f       // top left
};

///Corresponding indices
///for the triangles
///this info is for the
///EBO
unsigned int indices[] =
{
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

///Declare Window
GLFWwindow* window;

///Declare VAO, VBO and EBO
//VAO-Vertex Array Object
//VBO-Vertex Buffer Object
//EBO-Element Buffer Object
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

///This is the callback function for when the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///Just adjust the viewport to the new window size
    glViewport(0, 0, width, height);
}

///This is the callback function for input data, keyboard, mouse etc
void processInput(GLFWwindow *window)
{
    ///If the 'esc' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ///Close the window
        glfwSetWindowShouldClose(window, true);
    }

}

///\///////////////////INITIALIZE ALL THE FRAMEWORKS////////////////////////////
void initialize()
{
    ///Initialize GLFW
    if(glfwInit() == GLFW_TRUE)
    {
        cout << "GLFW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLFW" << endl;
    }

    ///Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///Uncomment this function for MAC-OS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ///Create a window
    window = glfwCreateWindow(800, 600, "ModernOpenGL", NULL, NULL);
    if (window == NULL)
    {
        ///If it fails, end the program
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    ///Set the current context, the window we are currently working in.
    glfwMakeContextCurrent(window);

    ///Register 'framebuffer_size_callback' as the resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ///Set the initial viewport
    glViewport(0, 0, 800, 600);

    ///Initialize GLEW
    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLEW" << endl;
    }


}
///\////////////////////////////////////////////////////////////////////////////


///\////////////////CREATION OF VAOs VBOs and EBOs//////////////////////////////
void setBufferObjects()
{
    ///Generate VBO
    glGenBuffers(1, &VBO);

    ///Generate VAO
    glGenVertexArrays(1, &VAO);

    ///Generate EBO
    glGenBuffers(1, &EBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}
///\////////////////////////////////////////////////////////////////////////////

void drawSquare(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(VAO);

    ///Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main ()
{
    ///Initialize all the frameworks
    initialize();

    ///Compile and Link Shaders into Shader Program
    Shader shader("shaders/vShader.vs","shaders/fShader.fs");

    ///Set all the info regarding buffer Objects
    setBufferObjects();

    ///Draw in Wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///Draw in Fillmode, this is default.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ///This is the render loop *While the window is open*
    while(!glfwWindowShouldClose(window))
    {
        ///Set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ///Draw
        drawSquare(shader);

        ///Process user input, in this case if the user presses the 'esc' key
        processInput(window);

        ///Swap the Front and Back buffer.
        glfwSwapBuffers(window);

        ///Poll CallBack Events
        glfwPollEvents();
    }

    ///Free resources when application ends.
    glfwTerminate();
    return 0;
}
