//
// Created by Lenovo on 2024/8/12.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Model testModel("../resources/models/Marry/Marry.obj");
    Shader BlinnPhongShader("../resources/shaders/BlinnPhong.vert","../resources/shaders/BlinnPhong.frag");
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    testModel.Destroy();
    BlinnPhongShader.Destroy();
    glfwTerminate();
    return 0;
}