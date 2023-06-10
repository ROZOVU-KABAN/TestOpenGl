#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"Renderer/ShaderProgram.h"
#include "Resources/ResourcesManager.h"
#include "Renderer/Texture2D.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


GLfloat point[] = {
    0.0f,50.f,0.0f,
    50.f,-50.f,0.0f,
    -50.f,-50.f,0.0f
};

GLfloat colors[] ={
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f
};

GLfloat texCoord[] = {
    0.5f,1.0f,
    1.0f,0.0f,
    0.0f,0.0f
};


glm::ivec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
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
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Let the BAttle Begin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "No Gl load";
        return -1;
    }

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version GL: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(123,234 ,187, 1);


    {
        ResourcesManager resourcesManager(argv[0]);
        auto pDefaultShaderProgram = resourcesManager.loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");

        if (!pDefaultShaderProgram)
        {
            std::cerr << "Error Build Default Shader Program\n";
            return -1;
        }

        auto tex = resourcesManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);


        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);


        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->SetInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));
        
        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 430.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);


        pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
    
            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);


            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}