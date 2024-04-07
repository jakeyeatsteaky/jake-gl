#include "Utility.hpp"

#include <iostream>


void util::DestroyWindow(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Successfully destroyed and terminated GLFW window" << std::endl;
}