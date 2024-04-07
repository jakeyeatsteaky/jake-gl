#ifndef APP_HPP
#define APP_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "Utility.hpp"
#include "Renderer.hpp"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


class App
{
private:
    // members
    int InitRenderer();
    int InitWindow();
    
    // methods
    bool m_isRunning;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<GLFWwindow, decltype(&util::DestroyWindow)> m_window;

public:
    App();
    ~App();
    void Run();
    bool IsRunning();
    void Setup();
    void Input();
    void Update();
    void Render();

};

#endif // end app.hpp