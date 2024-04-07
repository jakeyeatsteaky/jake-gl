#ifndef APP_HPP
#define APP_HPP

#include <memory>

#include "Renderer.hpp"

class App
{
private:
    bool m_isRunning;
    std::shared_ptr<Renderer> m_renderer;

    int InitRenderer();


public:
    App();
    ~App();
    bool IsRunning();
    void Setup();
    void Input();
    void Update();
    void Render();

};

#endif // end app.hpp