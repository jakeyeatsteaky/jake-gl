#include "App.hpp"
#include "Utility.hpp"

#include <iostream>

App::App()
{

}

App::~App()
{
    std::cout << "Destroying App Object" << std::endl;
}

bool App::IsRunning()
{
    return m_isRunning;
}

void App::Setup()
{
    int success = 0;
    success = InitRenderer();

    if(success < 0)
        std::cout << "Failed to init" << std::endl;
    
    return;

}

void App::Input()
{


}

void App::Update()
{


}

void App::Render()
{


}

// private
int App::InitRenderer()
{
    int success = util::eERR_FAILED;



}