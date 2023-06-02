#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Renderer/ShaderProgram.h"
#include "Resources/ResourcesManager.h"


GLfloat point[] = {
    0.0f,0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    -0.5f,-0.5f,0.0f
};

GLfloat colors[] ={
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f
};


int windowSizeX=640;
int windowSizeY=480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    windowSizeX = width;
    windowSizeY = height;
    glViewport(0, 0, windowSizeX, windowSizeY);
}


void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanecode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}


int main(int argc, char** argv)
{
 

    ResourcesManager resourcesManger(argv[0]);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //State version glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(windowSizeX, windowSizeY, "Let the BAttle Begin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "No Gl load";
        return -1;
    }


    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version GL: " << glGetString(GL_VERSION) << std::endl;


    glClearColor(1, 1,0 ,1);


    std::string vertexShader;// (vertex_shader);
    std::string fragmentShader;// (fragment_shader);
    Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);
    if (!shaderProgram.isCompiled())
    {
        std::cerr << "Error create shader program" << std::endl;
        return -1;
    }

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);


    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
   

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,nullptr);


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}