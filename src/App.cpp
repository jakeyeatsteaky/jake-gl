#include "App.hpp"
#include "Utility.hpp"

#include <iostream>
#include <thread>
#include <chrono>

App::App() :
    m_renderer(nullptr),
    m_window(nullptr, util::DestroyWindow)
{

}

App::~App()
{
    std::cout << "Destroying App Object" << std::endl;
}

void App::Run()
{
    while(IsRunning())
    {
      Input();
      Update();
      Render();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "Exiting main loop" << std::endl;
    
}

bool App::IsRunning()
{
    return m_isRunning;
}

void App::Setup()
{
    int success = 0;
    success = InitWindow();
    success = InitRenderer();

    if(success < 0)
        std::cout << "Failed to init renderer" << std::endl;
    
    return;

}

void App::Input()
{
    glfwPollEvents();
    if(glfwWindowShouldClose(m_window.get()))
        m_isRunning = false;

}

void App::Update()
{


}

void App::Render()
{


}

// private

int App::InitWindow()
{
    int success = util::eERR_FAILED;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    std::unique_ptr<GLFWwindow, decltype(&util::DestroyWindow)> pWindow (
        glfwCreateWindow(WIDTH, 
                         HEIGHT, 
                         "3D Renderer", 
                         nullptr, 
                         nullptr),
        util::DestroyWindow);

    m_window = std::move(pWindow);
 

    return success;
}


int App::InitRenderer()
{
    int success = util::eERR_FAILED;
    return success;

}
